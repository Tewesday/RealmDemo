

#pragma once

#include "CoreMinimal.h"
#include "MassCommands.h"
#include "MassCommandBuffer.h"
#include "MassCommonTypes.h"
#include "StructUtils.h"
#include "StructView.h"
#include "UObject/NoExportTypes.h"
#include <variant>

#include "EntityActorFragments.h"
#include "../Generic/EntityDescription.h"
#include "AssetFileSettings.h"

#include "ActorEntityDescription.generated.h"

USTRUCT()
struct REALMDEMO_API FActorEntityDescription : public FIEntityDescription {
    GENERATED_BODY()

    FActorEntityDescription();
    virtual ~FActorEntityDescription();

    TArray<uint32> ActorTags;
    TArray<uint32> ActorFragments;

    TTuple<AActor*, uint32> ActorFragmentData;

    static AActor* GetFragmentDataActor(FActorEntityDescription EntityDescription);
    static void AddFragmentDataActor(AActor* ActorData, FActorEntityDescription& EntityDescription);
    static FConstStructView CreateFragmentActor(FActorEntityDescription EntityDescription);
    static FAActorFragment CreateActorFragment(FActorEntityDescription EntityDescription);

    static uint32 GetFragmentDataMesh(FActorEntityDescription EntityDescription);
    static void AddFragmentDataMesh(MeshTypes Mesh, FActorEntityDescription& EntityDescription);
    static FAMeshFragment CreateMeshFragment(FActorEntityDescription EntityDescription);

    static bool AddActorFragmentToDescription(std::variant<AActor*, uint32> ActorFragmentData, EEntityActorFragments FragmentEnum, FActorEntityDescription* EntityDescription);

    static FConstStructView GetActorFragmentFromDescription(uint32 FragmentIndex, FActorEntityDescription EntityDescription);

    virtual TArray<FConstStructView> GetAllFragmentsFromDescription() override;

    virtual void AddAllFragmentsFromDescriptionToEntity(FMassEntityHandle Entity, FMassExecutionContext& Context) override;
};