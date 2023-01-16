

#include "EntityDescriptionSys.h"

void UEntityDescriptionSys::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UEntityDescriptionSys::Deinitialize()
{
}

bool UEntityDescriptionSys::AddSpawnToEntityDescription(std::variant<FVector2D> SpawnLocation, ESpawnFragments SpawnFragmentEnum, FEntityDescriptionMessage& EntityDescriptionMessage)
{
    FSpawnEntityDescription* SpawnEntityDescription;

    int32 ResultIndex = EntityDescriptionMessage.FindEntityDescriptionType(EEntityDescriptions::Spawn);
    if (ResultIndex >= 0) {
        SpawnEntityDescription = (FSpawnEntityDescription*)EntityDescriptionMessage.GetEntityDescription(ResultIndex);
    } else {
        SpawnEntityDescription = new FSpawnEntityDescription();
    }

    if (FSpawnEntityDescription::AddSpawnFragmentToDescription(SpawnLocation, SpawnFragmentEnum, *SpawnEntityDescription)) {
        EntityDescriptionMessage.AddEntityDescription(EEntityDescriptions::Spawn, SpawnEntityDescription);
        return true;
    }

    return false;
}

bool UEntityDescriptionSys::AddMovementToEntityDescription(std::variant<FTransform, float, FVector2D> MovementData, EDataArgument DataArgument, EMovementFragments MovementFragmentEnum, FEntityDescriptionMessage& EntityDescriptionMessage)
{
    FMovementEntityDescription* MovementEntityDescription;

    int32 ResultIndex = EntityDescriptionMessage.FindEntityDescriptionType(EEntityDescriptions::Movement);
    if (ResultIndex >= 0) {
        MovementEntityDescription = (FMovementEntityDescription*)EntityDescriptionMessage.GetEntityDescription(ResultIndex);
    } else {
        MovementEntityDescription = new FMovementEntityDescription();
    }

    if (FMovementEntityDescription::AddMovementFragmentToDescription(MovementData, DataArgument, MovementFragmentEnum, MovementEntityDescription)) {
        EntityDescriptionMessage.AddEntityDescription(EEntityDescriptions::Movement, MovementEntityDescription);
        return true;
    }

    return false;
}

bool UEntityDescriptionSys::AddActorToEntityDescription(std::variant<AActor*, uint32> ActorData, EEntityActorFragments ActorFragmentEnum, FEntityDescriptionMessage& EntityDescriptionMessage)
{
    FActorEntityDescription* ActorEntityDescription;

    int32 ResultIndex = EntityDescriptionMessage.FindEntityDescriptionType(EEntityDescriptions::EntityActor);
    if (ResultIndex >= 0) {
        ActorEntityDescription = (FActorEntityDescription*)EntityDescriptionMessage.GetEntityDescription(ResultIndex);
    } else {
        ActorEntityDescription = new FActorEntityDescription();
    }

    
    if (FActorEntityDescription::AddActorFragmentToDescription(ActorData, ActorFragmentEnum, ActorEntityDescription)) {
        EntityDescriptionMessage.AddEntityDescription(EEntityDescriptions::EntityActor, ActorEntityDescription);
        return true;
    }

    return false;
}

FEntityDescriptionMessage UEntityDescriptionSys::CreateStandardEntityDescription(AActor* ActorData)
{
    if (RandomAuthoritySys == nullptr) {
        RandomAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<URandomAuthoritySys>();
    }
    
    FEntityDescriptionMessage EntityDescriptionMessage = FEntityDescriptionMessage();

    // Add standard movement
    std::variant<FTransform, float, FVector2D> MoveData(40.f);
    
    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::AgentRadius, EntityDescriptionMessage);
    MoveData = FVector2D(RandomAuthoritySys->GetRandomPosition(-2000, 2000));
    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Steering, EntityDescriptionMessage);
    MoveData = FVector2D(2.f);
    AddMovementToEntityDescription(MoveData, EDataArgument::Second, EMovementFragments::Steering, EntityDescriptionMessage);

    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Order, EntityDescriptionMessage);
    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Octree, EntityDescriptionMessage);
    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::FormationMember, EntityDescriptionMessage);

    // Add actor reference, actor will be assigned later
    AddActorToEntityDescription(ActorData, EEntityActorFragments::Actor, EntityDescriptionMessage);

    AddActorToEntityDescription(uint32(MeshTypes::BaseMesh), EEntityActorFragments::Mesh, EntityDescriptionMessage);

    return EntityDescriptionMessage;
}

FEntityDescriptionMessage UEntityDescriptionSys::CreateStandardEntityDescription(AActor* ActorData, float AgentRadius)
{
    if (RandomAuthoritySys == nullptr) {
        RandomAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<URandomAuthoritySys>();
    }

    FEntityDescriptionMessage EntityDescriptionMessage;

    // Add standard movement
    std::variant<FTransform, float, FVector2D> MoveData(AgentRadius);

    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::AgentRadius, EntityDescriptionMessage);
    MoveData = FVector2D(RandomAuthoritySys->GetRandomPosition(-200, 200));
    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Steering, EntityDescriptionMessage);
    MoveData = FVector2D(2.f);
    AddMovementToEntityDescription(MoveData, EDataArgument::Second, EMovementFragments::Steering, EntityDescriptionMessage);

    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Order, EntityDescriptionMessage);

    // Add actor reference, actor will be assigned later
    AddActorToEntityDescription(ActorData, EEntityActorFragments::Actor, EntityDescriptionMessage);

    AddActorToEntityDescription(uint32(MeshTypes::BaseMesh), EEntityActorFragments::Mesh, EntityDescriptionMessage);

    return EntityDescriptionMessage;
}

FEntityDescriptionMessage UEntityDescriptionSys::CreateStandardEntityDescription(AActor* ActorData, FVector2D SteeringLocation)
{
    FEntityDescriptionMessage EntityDescriptionMessage;

    // Add standard movement
    std::variant<FTransform, float, FVector2D> MoveData(40.f);

    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::AgentRadius, EntityDescriptionMessage);
    MoveData = SteeringLocation;
    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Steering, EntityDescriptionMessage);
    MoveData = FVector2D(2.f);
    AddMovementToEntityDescription(MoveData, EDataArgument::Second, EMovementFragments::Steering, EntityDescriptionMessage);

    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Order, EntityDescriptionMessage);

    // Add actor reference, actor will be assigned later
    AddActorToEntityDescription(ActorData, EEntityActorFragments::Actor, EntityDescriptionMessage);

    AddActorToEntityDescription(uint32(MeshTypes::BaseMesh), EEntityActorFragments::Mesh, EntityDescriptionMessage);

    return EntityDescriptionMessage;
}

FEntityDescriptionMessage UEntityDescriptionSys::CreateStandardEntityDescription(AActor* ActorData, FVector2D SteeringLocation, FVector2D MaxVelocity)
{
    FEntityDescriptionMessage EntityDescriptionMessage;

    // Add standard movement
    std::variant<FTransform, float, FVector2D> MoveData(40.f);

    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::AgentRadius, EntityDescriptionMessage);
    MoveData = SteeringLocation;
    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Steering, EntityDescriptionMessage);
    MoveData = MaxVelocity;
    AddMovementToEntityDescription(MoveData, EDataArgument::Second, EMovementFragments::Steering, EntityDescriptionMessage);

    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Order, EntityDescriptionMessage);

    // Add actor reference, actor will be assigned later
    AddActorToEntityDescription(ActorData, EEntityActorFragments::Actor, EntityDescriptionMessage);

    AddActorToEntityDescription(uint32(MeshTypes::BaseMesh), EEntityActorFragments::Mesh, EntityDescriptionMessage);

    return EntityDescriptionMessage;
}

FEntityDescriptionMessage UEntityDescriptionSys::CreateStandardEntityDescription(AActor* ActorData, float AgentRadius, FVector2D SteeringLocation, FVector2D MaxVelocity)
{
    FEntityDescriptionMessage EntityDescriptionMessage;

    // Add standard movement
    std::variant<FTransform, float, FVector2D> MoveData(AgentRadius);

    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::AgentRadius, EntityDescriptionMessage);
    MoveData = SteeringLocation;
    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Steering, EntityDescriptionMessage);
    MoveData = MaxVelocity;
    AddMovementToEntityDescription(MoveData, EDataArgument::Second, EMovementFragments::Steering, EntityDescriptionMessage);

    AddMovementToEntityDescription(MoveData, EDataArgument::First, EMovementFragments::Order, EntityDescriptionMessage);

    // Add actor reference, actor will be assigned later
    AddActorToEntityDescription(ActorData, EEntityActorFragments::Actor, EntityDescriptionMessage);

    AddActorToEntityDescription(uint32(MeshTypes::BaseMesh), EEntityActorFragments::Mesh, EntityDescriptionMessage);

    return EntityDescriptionMessage;
}
