

#pragma once

#include "CoreMinimal.h"
#include "MassCommands.h"
#include "MassCommandBuffer.h"
#include "MassCommonTypes.h"
#include "UObject/NoExportTypes.h"
#include "StructUtils.h"
#include "StructView.h"
#include <variant>

#include "EntityDescription.h"
#include "SpawnFragments.h"

#include "SpawnEntityDescription.generated.h"

/**
 *
 */
USTRUCT()
struct REALMDEMO_API FSpawnEntityDescription : public FIEntityDescription {
    GENERATED_BODY()

    FSpawnEntityDescription();
    virtual ~FSpawnEntityDescription();

    TArray<uint32> SpawnTags;
    TArray<uint32> SpawnFragments;

    TTuple<FVector2D> SpawnFragmentData;

    static FVector2D GetFragmentDataSpawn(FSpawnEntityDescription EntityDescription);
    static void AddFragmentDataSpawn(FVector2D SpawnPositionData, FSpawnEntityDescription& EntityDescription);
    static FConstStructView CreateFragmentSpawn(FSpawnEntityDescription EntityDescription);
    static FSSpawnFragment CreateSpawnFragment(FSpawnEntityDescription EntityDescription);

    static bool AddSpawnFragmentToDescription(std::variant<FVector2D> SpawnFragmentData, ESpawnFragments SpawnFragmentEnum, FSpawnEntityDescription& EntityDescription);

    static FConstStructView GetSpawnFragmentFromDescription(uint32 FragmentIndex, FSpawnEntityDescription EntityDescription);

    virtual TArray<FConstStructView> GetAllFragmentsFromDescription() override;

    virtual void AddAllFragmentsFromDescriptionToEntity(FMassEntityHandle Entity, FMassExecutionContext& Context) override;

};
