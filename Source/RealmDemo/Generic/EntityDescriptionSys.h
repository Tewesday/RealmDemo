

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "StructUtils.h"
#include "StructView.h"
#include "Misc/TVariant.h"
#include <variant>

#include "EntityDescription.h"
#include "../Representation/ActorEntityDescription.h"
#include "SpawnEntityDescription.h"
#include "../Movement/MovementEntityDescription.h"
#include "EntityDescriptionMessage.h"
#include "../Representation/AssetFileSettings.h"
#include "RandomAuthoritySys.h"

#include "EntityDescriptionSys.generated.h"

/**
 *
 */
UCLASS()
class REALMDEMO_API UEntityDescriptionSys : public UWorldSubsystem {
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;

    bool AddSpawnToEntityDescription(std::variant<FVector2D> SpawnLocation, ESpawnFragments SpawnFragmentEnum, FEntityDescriptionMessage& EntityDescriptionMessage);
    bool AddMovementToEntityDescription(std::variant<FTransform, float, FVector2D> MovementData, EDataArgument DataArgument, EMovementFragments MovementFragmentEnum, FEntityDescriptionMessage& EntityDescriptionMessage);
    bool AddActorToEntityDescription(std::variant<AActor*, uint32> ActorData, EEntityActorFragments ActorFragmentEnum, FEntityDescriptionMessage& EntityDescriptionMessage);
    bool AddMeshToEntityDescription(std::variant<AActor*, uint32> ActorData, EEntityActorFragments ActorFragmentEnum, FEntityDescriptionMessage& EntityDescriptionMessage);

    FEntityDescriptionMessage CreateStandardEntityDescription(AActor* ActorData);
    FEntityDescriptionMessage CreateStandardEntityDescription(AActor* ActorData, float AgentRadius);
    FEntityDescriptionMessage CreateStandardEntityDescription(AActor* ActorData, FVector2D SteeringLocation);
    FEntityDescriptionMessage CreateStandardEntityDescription(AActor* ActorData, FVector2D SteeringLocation, FVector2D MaxVelocity);
    FEntityDescriptionMessage CreateStandardEntityDescription(AActor* ActorData, float AgentRadius, FVector2D SteeringLocation, FVector2D MaxVelocity);

    // EntityDescriptionMessages added here will be immediately consumed by the EntityActorSys/Prc.
    // If you want to send an EntityDescriptionMessage to an Entity with a Spawn Fragment, use FSpawnEventMessage
    TArray<FEntityDescriptionMessage> EntityDescriptionMessages;


    URandomAuthoritySys* RandomAuthoritySys;
};
