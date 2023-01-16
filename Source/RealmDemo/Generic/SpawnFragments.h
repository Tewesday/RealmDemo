

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTypes.h"
#include "StructUtils.h"
#include "StructView.h"

#include "EntityDescriptionMessage.h"
#include "EntityDescription.h"
#include "../Representation/AssetFileSettings.h"

#include "SpawnFragments.generated.h"

UENUM()
enum class ESpawnFragments {
    Spawn,
    SpawnFragments
};

USTRUCT()
struct REALMDEMO_API FSSpawnFragment : public FMassFragment {
    GENERATED_BODY()

    FSSpawnFragment() = default;

    explicit FSSpawnFragment(FVector2D SpawnLocationTarget)
        : SpawnLocation(SpawnLocationTarget)
    {
    }

    // Time until next spawn
    UPROPERTY(EditAnywhere, Category = "")
    float SpawnCountdown = INFINITY;

    // Position of the spawn location
    UPROPERTY(EditAnywhere, Category = "")
    FVector2D SpawnLocation;

    // Queue of spawn entities/items (the descriptions for each entity item)
    TArray<FSpawnEventMessage> Items;
};