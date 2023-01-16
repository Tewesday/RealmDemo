

#pragma once

#include "CoreMinimal.h"
#include "MassCommands.h"
#include "MassCommonTypes.h"
#include "StructUtils.h"
#include "StructView.h"
#include "UObject/NoExportTypes.h"
#include <variant>

#include "../Movement/MovementFragments.h"
#include "../Generic/EntityDescription.h"
#include "../Generic/SpawnFragments.h"

#include "MovementEntityDescription.generated.h"

UENUM()
enum class EDataArgument {
    First,
    Second,
    Third,
    DataStages
};

/**
 *
 */
USTRUCT()
struct REALMDEMO_API FMovementEntityDescription : public FIEntityDescription {
    GENERATED_BODY()

    FMovementEntityDescription();
    virtual ~FMovementEntityDescription();

    TArray<uint32> MovementTags;
    TArray<uint32> MovementFragments;

    // FMTransform, FMAgentRadius, FMSteering(StartPos), FMSteering(MaxVelocity)
    TTuple<FTransform, float, FVector2D, FVector2D> MovementFragmentData;

    static FTransform GetFragmentDataTransform(FMovementEntityDescription EntityDescription);
    static void AddFragmentDataTransform(FTransform TransformData, FMovementEntityDescription& EntityDescription);
    static FConstStructView CreateFragmentTransform(FMovementEntityDescription EntityDescription);
    static FMTransformFragment CreateTransformFragment(FMovementEntityDescription EntityDescription);

    static float GetFragmentDataAgentRadius(FMovementEntityDescription EntityDescription);
    static void AddFragmentDataAgentRadius(float AgentRadiusData, FMovementEntityDescription& EntityDescription);
    static FConstStructView CreateFragmentAgentRadius(FMovementEntityDescription EntityDescription);
    static FMAgentRadiusFragment CreateAgentRadiusFragment(FMovementEntityDescription EntityDescription);

    static FVector2D GetFragmentDataSteeringStartPos(FMovementEntityDescription EntityDescription);
    static void AddFragmentDataSteeringStartPos(FVector2D SteeringStartPosData, FMovementEntityDescription& EntityDescription);

    static FVector2D GetFragmentDataSteeringMaxVelocity(FMovementEntityDescription EntityDescription);
    static void AddFragmentDataSteeringMaxVelocity(FVector2D SteeringMaxVelocityData, FMovementEntityDescription& EntityDescription);
    static FConstStructView CreateFragmentSteering(FMovementEntityDescription EntityDescription);
    static FMSteeringFragment CreateSteeringFragment(FMovementEntityDescription EntityDescription);

    static void AddFragmentOrder(FMovementEntityDescription& EntityDescription);
    static FMOrderFragment CreateOrderFragment(FMovementEntityDescription EntityDescription);

    static void AddFragmentOctree(FMovementEntityDescription& EntityDescription);
    static FMOctreeFragment CreateOctreeFragment(FMovementEntityDescription EntityDescription);

    static void AddFragmentFormationMember(FMovementEntityDescription& EntityDescription);
    static FMFormationMemberFragment CreateFormationMemberFragment(FMovementEntityDescription EntityDescription);

    // Adds a Fragment's data to the EntityDescription for future construction of Fragments for one Entity
    // Call for each argument for each Fragment the Entity needs
    // Arguments: Data to be added for a Fragment, Which constructor argument the data is for (some Fragments have multiple argument constructors), Enum matching the Fragment type, EntityDescription to modify
    static bool AddMovementFragmentToDescription(std::variant<FTransform, float, FVector2D> MovementFragmentData, EDataArgument DataArgument, EMovementFragments FragmentEnum, FMovementEntityDescription* EntityDescription);

    // Finish adding the Fragment with data for the first argument
    static bool AddMovementFragmentToDescriptionFirstArgument(std::variant<FTransform, float, FVector2D> MovementFragmentData, EMovementFragments FragmentEnum, FMovementEntityDescription* EntityDescription);
    // Finish adding the Fragment with data for the second argument
    static bool AddMovementFragmentToDescriptionSecondArgument(std::variant<FTransform, float, FVector2D> MovementFragmentData, EMovementFragments FragmentEnum, FMovementEntityDescription* EntityDescription);

    // Get a constructed Fragment for the Entity this EntityDescription describes
    // Should be called in a loop adding all Fragments to one Entity
    static FConstStructView GetMovementFragmentFromDescription(uint32 FragmentIndex, FMovementEntityDescription EntityDescription);

    virtual TArray<FConstStructView> GetAllFragmentsFromDescription() override;

    virtual void AddAllFragmentsFromDescriptionToEntity(FMassEntityHandle Entity, FMassExecutionContext& Context) override;
};