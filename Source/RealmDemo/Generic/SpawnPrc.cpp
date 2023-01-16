

#include "SpawnPrc.h"

USpawnPrc::USpawnPrc()
{
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void USpawnPrc::Initialize(UObject& Owner)
{
	// Access violation, not ready yet?
	// SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
}

void USpawnPrc::ConfigureQueries()
{
	SpawnEntityQuery.AddRequirement<FSSpawnFragment>(EMassFragmentAccess::ReadWrite);
	SpawnEntityQuery.RegisterWithProcessor(*this);
}

void USpawnPrc::Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context)
{
	if (SimTimeAuthoritySys == nullptr) {
		SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
	}

	DequeueSpawnEventMessages();

	SpawnEntityQuery.ForEachEntityChunk(EntitySubsystem, Context, [&, this](FMassExecutionContext& Context) {
		const TArrayView<FSSpawnFragment> EntitySpawningFragments = Context.GetMutableFragmentView<FSSpawnFragment>();

		// Non Tick Based processing

		// Tick Based processing

		if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
			return;
		}

		const int32 QueryLength = Context.GetNumEntities();

		for (int32 i = 0; i < QueryLength; ++i) {

			if (EntitySpawningFragments[i].Items.Num() > 0) {

				// Update countdown of next Entity
				if (EntitySpawningFragments[i].SpawnCountdown == INFINITY) {
					EntitySpawningFragments[i].SpawnCountdown = EntitySpawningFragments[i].Items[0].TimeRequiredToSpawn;
				}
				else {
					EntitySpawningFragments[i].SpawnCountdown -= SimTimeAuthoritySys->SimTimeAuthority.TickRate;
				}

				// Spawn Entity
				if (EntitySpawningFragments[i].SpawnCountdown >= 0) {

					UEntityActorSys* EntityActorSys = GetWorld()->GetSubsystem<UEntityActorSys>();
					AActor* Actor = EntityActorSys->SpawnActor();
					auto Entity = EntityActorSys->SpawnEntity(Actor);

					// Add all Fragments and starting data to Entity
					for (FIEntityDescription* EntityDescription : EntitySpawningFragments[i].Items[0].EntityDescription.EntityDescriptions) {
						EntityDescription->AddAllFragmentsFromDescriptionToEntity(Entity, Context);
					}

					// Remove from queue and Reset
					EntitySpawningFragments[i].Items.RemoveAt(0);
					EntitySpawningFragments[i].SpawnCountdown = INFINITY;
				}
			}
		}
	});

	LastTickCalculated = SimTimeAuthoritySys->SimTimeAuthority.CurrentTick;
}

TTuple<FMassEntityHandle, TArray<FConstStructView>> USpawnPrc::SpawnEntity(FEntityDescriptionMessage EntityDescriptionMessage)
{
	TArray<FConstStructView> Fragments;

	for (FIEntityDescription* EntityDescription : EntityDescriptionMessage.EntityDescriptions) {
		Fragments.Append(EntityDescription->GetAllFragmentsFromDescription());
	}

	UEntityActorSys* EntityActorSys = GetWorld()->GetSubsystem<UEntityActorSys>();
	AActor* Actor = EntityActorSys->SpawnActor();
	auto Entity = EntityActorSys->SpawnEntity(Actor);

	return MakeTuple(Entity, Fragments);
}

void USpawnPrc::DequeueSpawnEventMessages()
{
	if (SpawnSys == nullptr) {
		SpawnSys = GetWorld()->GetSubsystem<USpawnSys>();
	}

	for (FSpawnEventMessage SpawnEventMessage : SpawnSys->SpawnEventMessages) {
		if (SpawnEventMessage.MarkedForDeletion) {
			continue;
		}
		FMassEntityHandle Entity = SpawnEventMessage.SpawnEntityTarget;
		UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
		FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();
		FMassEntityView EntityView = FMassEntityView(*EntityManager, Entity);

		FSSpawnFragment* EntitySpawnFragment = EntityView.GetFragmentDataPtr<FSSpawnFragment>();
		FSpawnEventMessage CopyOfSpawnEventMessage = SpawnEventMessage;
		EntitySpawnFragment->Items.Add(CopyOfSpawnEventMessage);

		SpawnEventMessage.MarkedForDeletion = true;
	}

	int32 MessageCount = SpawnSys->SpawnEventMessages.Num();
	for (int32 Index = (MessageCount - 1); Index >= 0; --Index) {
		SpawnSys->SpawnEventMessages.RemoveAt(Index);
	}
	
}

void USpawnPrc::ChunkDequeueSpawnEventMessages(FMassEntityHandle Entity)
{
	// See if any relevant SpawnEventMessages exist
	int32 SpawnMessagesCount = SpawnSys->SpawnEventMessages.Num();
	if (SpawnMessagesCount > 0) {
		// auto LocalEntityHandle = Context.GetEntity(EntityIndex);

		for (int32 SpawnEventMessagesIndex = 0; SpawnEventMessagesIndex < SpawnMessagesCount; ++SpawnEventMessagesIndex) {
			auto SpawnEventMessage = SpawnSys->SpawnEventMessages[SpawnEventMessagesIndex];

			if (SpawnEventMessage.SpawnEntityTarget == Entity) {
				// Unload SpawnEventMessage
				// auto EntityDescription = SpawnEventMessage.EntityDescription;
				// SpawnsList[EntityIndex].Items.Add(EntityDescription);
				// SpawnEventMessage will be deleted by another processor when this one's execution is complete
				// SpawnSystem->MarkSpawnEventMessageForDeletion(SpawnEventMessagesIndex);
			}
		}
	}
}
