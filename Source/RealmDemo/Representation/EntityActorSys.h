

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "MassCommonTypes.h"
#include "MassEntitySubsystem.h"
#include "MassEntityTypes.h"
#include "MassEntityView.h"
#include "MassProcessor.h"
#include "StructUtils.h"
#include "StructView.h"
#include "Subsystems/WorldSubsystem.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

#include "RTSEntityActor.h"
#include "RTSEntityComponent.h"
#include "EntityActorFragments.h"
#include "../Generic/GenericTags.h"
#include "../Movement/MovementFragments.h"

#include "EntityActorSys.generated.h"

/**
 *
 */
UCLASS()
class REALMDEMO_API UEntityActorSys : public UWorldSubsystem {
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;

    // Called by Mass to get an Actor
    AActor* SpawnActor();

    // Destroys Actor without destroying Entity
    void DestroyActor(AActor* EntityActor);

    // Called by Mass to assign an Entity to an Actor
    FMassEntityHandle SpawnEntity(AActor* EntityActor);

    // Called by Mass to assign an Actor to an Entity
    AEntityActor* AssignActor(FMassEntityHandle Entity);

    // Destroys Entity and EntityActor
    void DestroyEntity(AActor* EntityActor);

    // Should be called after DestroyEntity
    void RemoveEntityActorFromPool(int32 Index);

    // Ensures that there are no invalid EntityActors hanging around in memory
    void CleanupEntityActorArrays();

    TArray<FMassEntityHandle> GetEntitiesInSphere(FSphere Sphere);

    FMassEntityView GetEntityView(FMassEntityHandle Entity);

    TArray<FMassEntityHandle> GetEntitiesHandles();

private:
    TArray<AEntityActor*> EntityActors;
    TArray<FMassEntityHandle> Entities;
    FMassArchetypeHandle GenericEntityArchetype;

    UEntityComponent* AqcuireEntityComponent(AActor* InActor);
};
