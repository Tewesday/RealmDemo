


#include "AssetSys.h"

#include "AssetFileSettings.h"

void UAssetSys::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UAssetFileSettings* AssetFileSettings = GetDefault<UAssetFileSettings>();

	auto BaseMesh = AssetFileSettings->BaseMesh.LoadSynchronous();
}

void UAssetSys::SetMesh(AActor* Actor, UStaticMesh* MeshTarget)
{
	auto StaticMeshComponent = AcquireStaticMeshComponent(Actor);

	StaticMeshComponent->SetStaticMesh(MeshTarget);
}

void UAssetSys::SetBaseMesh(AActor* Actor)
{
	auto StaticMeshComponent = AcquireStaticMeshComponent(Actor);

	const UAssetFileSettings* AssetFileSettings = GetDefault<UAssetFileSettings>();

	StaticMeshComponent->SetStaticMesh(AssetFileSettings->BaseMesh.Get());
}


UStaticMesh* UAssetSys::GetMesh(int32 MeshType)
{
	if (MeshType < MeshTypes::Types) {
		switch (MeshType)
		{
		case MeshTypes::BaseMesh: {
			return GetBaseMesh();
			break;
		}
		}
	}
	return nullptr;
}

UStaticMesh* UAssetSys::GetBaseMesh()
{
	const UAssetFileSettings* AssetFileSettings = GetDefault<UAssetFileSettings>();

	auto BaseMesh = AssetFileSettings->BaseMesh.LoadSynchronous();

	return AssetFileSettings->BaseMesh.Get();
}

UStaticMeshComponent* UAssetSys::AcquireStaticMeshComponent(AActor* InActor)
{
	UActorComponent* ActorComponent = InActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (ActorComponent != nullptr) {
		auto StaticMeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		return StaticMeshComponent;
	}

	return nullptr;
}

UStaticMeshComponent* UAssetSys::AcquireStaticMeshComponent(AActor* InActor, FName StaticMeshComponentName)
{
	TArray<UActorComponent*> ActorComponents;
	ActorComponents.Append(InActor->GetComponentsByClass(UStaticMeshComponent::StaticClass()));
	if (ActorComponents.Num() > 0) {
		for (int32 ActorComponentIndex = 0; ActorComponentIndex < ActorComponents.Num(); ++ActorComponentIndex) {
			auto StaticMeshComponent = Cast<UStaticMeshComponent>(ActorComponents[ActorComponentIndex]);
			if (StaticMeshComponent->GetFName() == StaticMeshComponentName) {
				return StaticMeshComponent;
			}
		}

	}

	return nullptr;
}

void UAssetSys::AddHISM(int32 MeshType)
{
	AActor* Actor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FActorSpawnParameters());
	HismActors.Add(Actor);

	UHierarchicalInstancedStaticMeshComponent* Hism = NewObject<UHierarchicalInstancedStaticMeshComponent>(Actor);
	Hism->RegisterComponent();
	// TODO IS THIS NEEDED??
	Actor->AddInstanceComponent(Hism);
	Hism->AttachToComponent(Actor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Hism->SetEnableGravity(false);
	Hism->SetSimulatePhysics(false);
	Hism->bDisableCollision = true;
	Hism->bAutoRebuildTreeOnInstanceChanges = false;
	Hism->bConcurrentChanges = true;

	Hism->SetStaticMesh(GetMesh(MeshType));

	Hisms.Add(Hism);
	HismsMap.Add(MeshType, Hism);
}

int32 UAssetSys::FindHISM(int32 MeshType)
{
	int32 Index = INDEX_NONE;
	for (auto* Hism : Hisms) {
		++Index;
		if (Hism->GetStaticMesh() == GetMesh(MeshType)) {
			return Index;
		}
	}
	return Index;
}

std::tuple<uint32, uint32> UAssetSys::AddHISMInstance(int32 MeshType, FTransform EntityTransform)
{
	auto* Hism = HismsMap[MeshType];

	uint32 IndexOfInstance = Hism->AddInstance(EntityTransform);

	return std::make_tuple(MeshType, IndexOfInstance);
}
