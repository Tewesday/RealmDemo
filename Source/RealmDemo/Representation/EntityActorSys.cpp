


#include "EntityActorSys.h"

void UEntityActorSys::Initialize(FSubsystemCollectionBase& Collection)
{
    UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
    FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();

    GenericEntityArchetype = EntityManager->CreateArchetype(TConstArrayView<const UScriptStruct*> {

    });
}

void UEntityActorSys::Deinitialize()
{
}

AActor* UEntityActorSys::SpawnActor()
{
    AEntityActor* Actor = GetWorld()->SpawnActor<AEntityActor>(AEntityActor::StaticClass(), FActorSpawnParameters());
    EntityActors.Add(Actor);
    return Actor;
}

void UEntityActorSys::DestroyActor(AActor* EntityActor)
{
    EntityActor->Destroy();
}

FMassEntityHandle UEntityActorSys::SpawnEntity(AActor* EntityActor)
{
    UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
    FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();

    FMassEntityHandle Entity = EntityManager->CreateEntity(GenericEntityArchetype);

    EntityManager->CheckIfEntityIsValid(Entity);
    EntityManager->CheckIfEntityIsActive(Entity);

    int32 EntityActorIndex = Entities.Add(Entity);

    // Assign EntityHandle to EntityActor's component
    UEntityComponent* Component = AqcuireEntityComponent(EntityActor);

    if (Component != nullptr) {
        Component->SetEntityHandle(Entity);
        Component->SetEntityActorIndex(EntityActorIndex);
    } else {
        EntityManager->DestroyEntity(Entity);
        // NOT a valid entity
        Entities.RemoveAt(EntityActorIndex);
    }

    return Entity;
}

AEntityActor* UEntityActorSys::AssignActor(FMassEntityHandle Entity)
{
    for (AEntityActor* Actor : EntityActors) {
        auto* EntityComponent = Actor->GetEntityComponent();
        if (Entity == EntityComponent->GetEntityHandle()) {
            UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
            FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();
            FMassEntityView EntityView = FMassEntityView(*EntityManager, Entity);

            FAActorFragment* ActorFragment = EntityView.GetFragmentDataPtr<FAActorFragment>();
            ActorFragment->Actor = Actor;
            return Actor;
        }
    }
    return nullptr;
}

void UEntityActorSys::DestroyEntity(AActor* EntityActor)
{
    UEntityComponent* Component = AqcuireEntityComponent(EntityActor);

    if (Component != nullptr) {
        UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
        FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();

        EntityManager->DestroyEntity(Component->GetEntityHandle());

        EntityActor->Destroy();
    }
}

void UEntityActorSys::RemoveEntityActorFromPool(int32 Index)
{
    EntityActors.RemoveAt(Index);
    Entities.RemoveAt(Index);
}

void UEntityActorSys::CleanupEntityActorArrays()
{
    TArray<FMassEntityHandle> NewEntities;
    TArray<AEntityActor*> NewEntityActors;

    UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
    FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();

    for (int32 EntityActorIndex = 0; EntityActorIndex < EntityActors.Num(); ++EntityActorIndex) {
        auto Entity = Entities[EntityActorIndex];

        if (!EntityManager->IsEntityValid(Entity)) {
            continue;
        }

        FMassEntityView EntityView = FMassEntityView(*EntityManager, Entity);

        if (EntityView.HasTag<FNeedsCleanup>()) {
            continue;
        }

        // If the entitiy is valid and not dead, move it to the new arrays
        int32 NewIndex = NewEntities.Add(Entity);

        FAActorFragment* ActorFragment = EntityView.GetFragmentDataPtr<FAActorFragment>();
        AEntityActor* Actor = Cast<AEntityActor>(ActorFragment->Actor);
        Actor->GetEntityComponent()->SetEntityActorIndex(NewIndex);

        NewEntityActors.Add(Actor);
    }

    Entities.Empty();
    EntityActors.Empty();
    Entities.Append(NewEntities);
    EntityActors.Append(NewEntityActors);

    NewEntities.Empty();
    NewEntityActors.Empty();
}

TArray<FMassEntityHandle> UEntityActorSys::GetEntitiesInSphere(FSphere Sphere)
{
    UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
    FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();

    TArray<FMassEntityHandle> FoundEntities;

    int32 EntityCount = Entities.Num();

    for (int32 EntityIndex = 0; EntityIndex < EntityCount; ++EntityIndex) {
        auto Entity = Entities[EntityIndex];


        if (!EntityManager->IsEntityValid(Entity)) {
            continue;
        }

        FMassEntityView EntityView = FMassEntityView(*EntityManager, Entity);

        //if (!EntityView.HasTag<FHasSteeringTag>()) {
        //    continue;
        //}
        auto SteeringFragment = EntityView.GetFragmentData<FMSteeringFragment>();

        // Filter out Entities that do not belong to the player
        //auto TeamFragment = EntityView.GetFragmentData<FRTSTeamFragment>();
        //if (TeamFragment.Team != ETeams::Team1) {
        //    continue;
        //}

        FSphere EntitySphere = FSphere(FVector3d(SteeringFragment.Position, 0.f), 80.f);

        //Util::PrintDebugMessageToScreen(FString(EntityBox.ToString()));

        if (Sphere.IsInside(EntitySphere)) {
            FoundEntities.Add(Entity);
        }
        else if (Sphere.Intersects(EntitySphere)) {
            FoundEntities.Add(Entity);
        }
        else {
            //Util::PrintDebugMessageToScreen(FString("No intersection or inside"));
        }
    }
    return FoundEntities;
}

FMassEntityView UEntityActorSys::GetEntityView(FMassEntityHandle Entity)
{
    UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
    FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();
    FMassEntityView EntityView = FMassEntityView(*EntityManager, Entity);

    return EntityView;
}

TArray<FMassEntityHandle> UEntityActorSys::GetEntitiesHandles()
{
    return Entities;
}

UEntityComponent* UEntityActorSys::AqcuireEntityComponent(AActor* InActor)
{
    UActorComponent* ActorComponent = InActor->GetComponentByClass(UEntityComponent::StaticClass());
    if (ActorComponent != nullptr) {
        UEntityComponent* EntityComponent = Cast<UEntityComponent>(ActorComponent);
        return EntityComponent;
    }

    return nullptr;
}