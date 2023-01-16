

#pragma once

#include "CoreMinimal.h"
#include "MassCommonTypes.h"
#include "MassEntityTypes.h"
#include "StructUtils.h"
#include "StructView.h"

#include "EntityDescription.h"

#include "EntityDescriptionMessage.generated.h"

/**
 *
 */
USTRUCT()
struct REALMDEMO_API FEntityDescriptionMessage {
    GENERATED_BODY()

    // Compromise.
    // List of EntityDescriptionEnums so that it can be determined which EntityDescription is what type in EntityDescriptions.
    TArray<uint32> EntityDescriptionEnums;

    // All of the EntityDescriptions needed for this Entity
    TArray<FIEntityDescription*> EntityDescriptions;

    void AddEntityDescription(EEntityDescriptions EntityDescriptionTypeEnum, FIEntityDescription* EntityDescription);
    // Returns the index in both arrays that matches the Enum type searched for. Negative if none found.
    int32 FindEntityDescriptionType(EEntityDescriptions EntityDescriptionTypeEnum);

    // Returns the EntityDescription after getting the index from FindEntityDescriptionType.
    FIEntityDescription* GetEntityDescription(uint32 IndexOfDescription);
};

// TODO: MOVE TO Spawn Module
USTRUCT()
struct REALMDEMO_API FSpawnEventMessage {
    GENERATED_BODY()

    FMassEntityHandle SpawnEntityTarget;

    FEntityDescriptionMessage EntityDescription;

    double TimeRequiredToSpawn;

    bool MarkedForDeletion = false;

};
