

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"



#include "AssetSys.generated.h"


enum MeshTypes;

/**
 *
 */
UCLASS()
class REALMDEMO_API UAssetSys : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void SetMesh(AActor* Actor, UStaticMesh* MeshTarget);

	void SetBaseMesh(AActor* Actor);

	UStaticMesh* GetMesh(int32 MeshType);

	UStaticMesh* GetBaseMesh();


	UStaticMeshComponent* AcquireStaticMeshComponent(AActor* InActor);

	UStaticMeshComponent* AcquireStaticMeshComponent(AActor* InActor, FName StaticMeshComponentName);

	TMap<int32, UHierarchicalInstancedStaticMeshComponent*> HismsMap;

	void AddHISM(int32 MeshType);

	int32 FindHISM(int32 MeshType);

	std::tuple<uint32, uint32> AddHISMInstance(int32 MeshType, FTransform EntityTransform);

protected:

private:

	//const UAssetFileSettings* AssetFileSettings = GetDefault<UAssetFileSettings>();

	TArray<UHierarchicalInstancedStaticMeshComponent*> Hisms;

	TArray<AActor*> HismActors;

	


};
