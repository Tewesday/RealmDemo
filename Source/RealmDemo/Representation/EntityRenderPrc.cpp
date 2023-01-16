


#include "EntityRenderPrc.h"

void UEntityRenderPrc::Initialize(UObject& Owner)
{
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	ExecutionOrder.ExecuteAfter.Add(TEXT("EntityActorPrc"));
	ExecutionOrder.ExecuteAfter.Add(TEXT("SpawnPrc"));
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UEntityRenderPrc::ConfigureQueries()
{
	ActorMeshQuery.AddRequirement<FAActorFragment>(EMassFragmentAccess::ReadWrite);
	ActorMeshQuery.AddRequirement<FAMeshFragment>(EMassFragmentAccess::ReadWrite);
	ActorMeshQuery.RegisterWithProcessor(*this);

	ActorSceneQuery.AddRequirement<FAActorFragment>(EMassFragmentAccess::ReadWrite);
	ActorSceneQuery.AddRequirement<FAMeshFragment>(EMassFragmentAccess::ReadOnly);
	ActorSceneQuery.AddRequirement<FMSteeringFragment>(EMassFragmentAccess::ReadOnly);
	ActorSceneQuery.RegisterWithProcessor(*this);
}

void UEntityRenderPrc::Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context)
{
	if (SimTimeAuthoritySys == nullptr) {
		SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
	}

	if (AssetSys == nullptr) {
		AssetSys = GetWorld()->GetGameInstance()->GetSubsystem<UAssetSys>();

		// Add any missing HISMs
		for (uint32 MeshIndex = 0; MeshIndex < MeshTypes::Types; ++MeshIndex) {
			if (AssetSys->FindHISM(MeshIndex) == INDEX_NONE) {
				AssetSys->AddHISM(MeshIndex);
			}
		}
	}

	ActorMeshQuery.ForEachEntityChunk(EntitySubsystem, Context,
		[&, this](FMassExecutionContext& Context) {
			const TArrayView<FAActorFragment> EntityActorFragments = Context.GetMutableFragmentView<FAActorFragment>();
			const TArrayView<FAMeshFragment> EntityMeshFragments = Context.GetMutableFragmentView<FAMeshFragment>();

			// Non Tick Based processing
			const int32 QueryLength = Context.GetNumEntities();

			for (int32 i = 0; i < QueryLength; ++i) {

				// Update mesh
				if (!EntityMeshFragments[i].MeshUpToDate) {

					// Not ready yet
					if (EntityActorFragments[i].Actor == nullptr) {
						continue;
					}

					if (EntityMeshFragments[i].UsesHism) {
						// Setup HISM Instance
						EntityMeshFragments[i].HismIndex = AssetSys->AddHISMInstance(EntityMeshFragments[i].MeshType, FTransform());

					}
					else {
						// Setup SMC
						auto* StaticMeshComp = AssetSys->AcquireStaticMeshComponent(EntityActorFragments[i].Actor);

						StaticMeshComp->SetStaticMesh(AssetSys->GetMesh(EntityMeshFragments[i].MeshType));

						EntityMeshFragments[i].ActorSceneComponent = StaticMeshComp;
					}

					EntityMeshFragments[i].ActorScale3D = EntityActorFragments[i].Actor->GetActorScale3D();

					EntityMeshFragments[i].MeshUpToDate = true;
				}
			}

			// Tick Based processing

			if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
				return;
			}

			for (int32 i = 0; i < QueryLength; ++i) {
			}
		});

	TArray<std::tuple<uint32, uint32, FTransform>> TransformUpdates;

	ActorSceneQuery.ForEachEntityChunk(EntitySubsystem, Context,
		[&, this](FMassExecutionContext& Context) {
			const TArrayView<FAActorFragment> EntityActorFragments = Context.GetMutableFragmentView<FAActorFragment>();
			const TConstArrayView<FAMeshFragment> EntityMeshFragments = Context.GetFragmentView<FAMeshFragment>();
			const TConstArrayView<FMSteeringFragment> EntitySteeringFragments = Context.GetFragmentView<FMSteeringFragment>();


			// Non Tick Based processing

			const int32 QueryLength = Context.GetNumEntities();

			for (int32 i = 0; i < QueryLength; ++i) {
				// Not ready yet
				if (EntityActorFragments[i].Actor == nullptr) {
					continue;
				}

				// Calculate new Transform position
				FVector2D SteeringPos = EntitySteeringFragments[i].Position;

				FRotator Rotator = FRotator::ZeroRotator;
				Rotator = UKismetMathLibrary::FindLookAtRotation(FVector(SteeringPos, 0.f), FVector(EntitySteeringFragments[i].Target, 0.f));

				// Update the entity's SM transform
				FTransform EntityTransform = FTransform(Rotator, FVector(SteeringPos, 0.f), EntityActorFragments[i].Actor->GetActorScale3D());

				if (EntityMeshFragments[i].UsesHism) {
					// HISM update method
					TransformUpdates.Add(std::make_tuple(std::get<0>(EntityMeshFragments[i].HismIndex), std::get<1>(EntityMeshFragments[i].HismIndex), EntityTransform));

				}
				else {
					EntityActorFragments[i].Actor->SetActorLocationAndRotation(FVector(SteeringPos, 0.f), Rotator);
				}

			}

			// Tick Based processing

			if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
				return;
			}



			for (int32 i = 0; i < QueryLength; ++i) {

			}


		});


	AsyncTask(ENamedThreads::GameThread, [this, TransformUpdates]() {
		for (auto TransformUpdate : TransformUpdates) {
			uint32 HismIndex = std::get<0>(TransformUpdate);
			uint32 HismInstanceIndex = std::get<1>(TransformUpdate);
			FTransform EntityTransform = std::get<2>(TransformUpdate);

			AssetSys->HismsMap[HismIndex]->UpdateInstanceTransform(HismInstanceIndex, EntityTransform, true, false, false);
		}
		AssetSys->HismsMap[MeshTypes::BaseMesh]->BuildTreeIfOutdated(true, true);
		AssetSys->HismsMap[MeshTypes::BaseMesh]->MarkRenderTransformDirty();
	});

	LastTickCalculated = SimTimeAuthoritySys->SimTimeAuthority.CurrentTick;
}
