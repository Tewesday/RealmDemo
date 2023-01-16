#pragma once

#include "CoreMinimal.h"
#include "MassCommonTypes.h"
#include "MassEntityTypes.h"
#include "StructUtils.h"
#include "StructView.h"

#include "../Generic/octree.h"
#include "../Generic/octree_container.h"
#include "../Generic/Grid.h"

#include "MovementFragments.generated.h"


UENUM()
enum class EMovementFragments {
    Transform,
    AgentRadius,
    Steering,
    Order,
    Octree,
    Formation,
    FormationMember,
    Grid,
    MovementFragments
};

UENUM()
enum class EMoveOrderType {
    Idle,
    MoveTo,
    Chase,
    MoveOrders
};

USTRUCT()
struct REALMDEMO_API FMTransformFragment : public FMassFragment {
    GENERATED_BODY()

    FMTransformFragment() = default;

    explicit FMTransformFragment(FTransform TransformTarget)
        : Transform(TransformTarget)
    {
    }

    FTransform Transform = FTransform();
};

USTRUCT()
struct REALMDEMO_API FMAgentRadiusFragment : public FMassFragment {
    GENERATED_BODY()

    FMAgentRadiusFragment() = default;

    explicit FMAgentRadiusFragment(float RadiusTarget)
        : Radius(RadiusTarget)
    {
    }

    inline static const float DEFAULT_RADIUS = 40.f;

    float Radius = DEFAULT_RADIUS;
};

USTRUCT()
struct REALMDEMO_API FMSteeringFragment : public FMassFragment {
    GENERATED_BODY()

    FMSteeringFragment() = default;

    explicit FMSteeringFragment(FVector2D PositionTarget, FVector2D MaxVelocityTarget)
        : Position(PositionTarget)
        , MaxVelocity(MaxVelocityTarget)
    {
    }

    explicit FMSteeringFragment(FVector2D PositionTarget)
        : Position(PositionTarget)
    {
    }

    bool HasStartPositionBeenSetByTransform = false;

    inline static const FVector2D DEFAULT_POSITION = FVector2D(0.f);

    FVector2D Position = DEFAULT_POSITION;

    inline static const FVector2D DEFAULT_VELOCITY = FVector2D(0.f);

    FVector2D Velocity = DEFAULT_VELOCITY;

    inline static const FVector2D DEFAULT_TARGET = FVector2D(0.f);

    FVector2D Target = DEFAULT_TARGET;

    inline static const FVector2D DEFAULT_MAX_VELOCITY = FVector2D(2.f);

    FVector2D MaxVelocity = DEFAULT_MAX_VELOCITY;

    inline static const FVector2D DEFAULT_DIRECTION = FVector2D::UnitVector;

    FVector2D Direction = DEFAULT_DIRECTION;

    // Series of goals that become target destinations
    TArray<FVector2D> WaypointGoals;

    bool ControlledByFormation = false;

    OrthoTree::BoundingBox2D BoundingBox;


    // Used to track if the steering agent hasn't moved
    float TimeBarelyMoved = 0.f;
    FVector2D LastNotablePosition = Position;

    bool NeedsOctreeUpdate = false;

    // The tile this fragment is currently within (if using a grid)
    FMassEntityHandle Tile;
};

USTRUCT()
struct REALMDEMO_API FMOrderFragment : public FMassFragment
{
    GENERATED_BODY()

    FMOrderFragment() = default;

    EMoveOrderType Order = EMoveOrderType::Idle;
    FVector2D OrderTargetPosition;

    // Only used if chasing a target
    FMassEntityHandle OrderTargetEntity;

    bool OrderNeedsUpdate = false;

};

USTRUCT()
struct REALMDEMO_API FMOctreeFragment : public FMassFragment
{
    GENERATED_BODY()

    FMOctreeFragment() = default;

    uint32 OctreeID;

    uint32 PointTreeID;
    
    bool OctreeNeedsUpdate = false;

    bool NeedsFirstOctreeInsertion = true;

};

USTRUCT()
struct REALMDEMO_API FMFormationFragment : public FMassFragment
{
    GENERATED_BODY()

    FMFormationFragment() = default;

    TArray<FMassEntityHandle> EntitiesInFormation;

    TArray<FMassEntityHandle> EntitiesRequestingToJoin;

    TArray<FMassEntityHandle> EntitiesRequestingToLeave;

    uint32 RowsInFormation = 0;
    uint32 ColsInFormation = 0;

    TArray<uint32> ColsInEachRow;

    Grid FormationGrid;

    inline static const FVector2D DEFAULT_POSITION = FVector2D(0.f);

    FVector2D Position = DEFAULT_POSITION;

    FVector2D LastPosition = DEFAULT_POSITION;

    FVector2D FromLastToCurrentPos = Position - LastPosition;

    inline static const FVector2D DEFAULT_TARGET = FVector2D(0.f);

    FVector2D Target = DEFAULT_TARGET;

    inline static const FVector2D DEFAULT_VELOCITY = FVector2D(0.f);

    FVector2D Velocity = DEFAULT_VELOCITY;

    inline static const FVector2D DEFAULT_MAX_VELOCITY = FVector2D(2.f);

    FVector2D MaxVelocity = DEFAULT_MAX_VELOCITY;

    inline static const FVector2D DEFAULT_DIRECTION = FVector2D::UnitVector;

    FVector2D Direction = DEFAULT_DIRECTION;

    inline static const float DEFAULT_RADIUS = 25.f;

    float UnitRadius = DEFAULT_RADIUS;

    inline static const float DEFAULT_TURN_RADIUS = 25.f;

    float TurnRadius = DEFAULT_TURN_RADIUS;

};

USTRUCT()
struct REALMDEMO_API FMFormationMemberFragment : public FMassFragment {
    GENERATED_BODY()

    FMFormationMemberFragment() = default;

    uint32 FormationIndex = 0;

    bool InFormation = false;

    inline static const FVector2D DEFAULT_OFFSET = FVector2D(0.f);

    FVector2D OffsetFromCenterOfFormation = DEFAULT_OFFSET;

    FVector2D OffsetTarget = DEFAULT_OFFSET;
};

// TODO: MOVE THIS?
USTRUCT()
struct REALMDEMO_API FMGridFragment : public FMassFragment {
    GENERATED_BODY()

    FMGridFragment() = default;


};