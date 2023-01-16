

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassCommonTypes.h"
#include "MassEntityView.h"
#include "VectorTypes.h"

#include "../Movement/MovementFragments.h"
#include "../Generic/SimTimeAuthoritySubsystem.h"
#include "../Generic/RandomAuthoritySys.h"
#include "../Generic/OctreeSys.h"
#include "../Generic/octree.h"
#include "../Generic/octree_container.h"

#include "SteeringPrc.generated.h"

DECLARE_STATS_GROUP(TEXT("FindEntities"), STATGROUP_FINDENTITIES, STATCAT_Advanced);
DECLARE_STATS_GROUP(TEXT("Avoidance"), STATGROUP_AVOIDANCE, STATCAT_Advanced);

/**
 *
 */
UCLASS()
class REALMDEMO_API USteeringPrc : public UMassProcessor {
    GENERATED_BODY()

    USteeringPrc();

    virtual void Initialize(UObject& Owner) override;
    virtual void ConfigureQueries() override;
    virtual void Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context) override;

    void UpdateTarget(FVector2D Target, FMSteeringFragment& SteeringFragment);

    // Calculate steering forces from current position towards target position
    FVector2D CalculateSteeringTowardsTarget(FVector2D Target, FMSteeringFragment& SteeringFragment);

    FVector2D CalculateAvoidanceAroundSelf(const FMassEntityHandle SelfHandle, FMSteeringFragment& SteeringFragment);

    std::vector<OrthoTree::entity_id_type> FindEntitiesAroundSelfInRadius(FVector2D Position, float Radius);
    std::vector<OrthoTree::entity_id_type> FindEntitiesAroundSelf(FMSteeringFragment& SteeringFragment);

    std::vector<OrthoTree::entity_id_type> FindNearestEntitiesAroundSelfPoint(FMSteeringFragment& SteeringFragment);

    // Apply new steering forces to velocity and update position
    void UpdatePosition(FVector2D Steering, FMSteeringFragment& SteeringFragment);

    // Notify destination has been reached
    bool IsDestinationReached(FMSteeringFragment& SteeringFragment);

    // Create a new FVector2D with a random position
    FVector2D GetARandomPosition(FRandomStream& RandomStream, double Min, double Max);

    FVector2D GetTargetBasedOnEntityTarget(FMassEntityManager& EntityManager, FMassEntityHandle EntityTarget);

    FVector2D GetAndUpdateFacingDirection(FMSteeringFragment& SteeringFragment);

    FVector2D GetFacingDirection(const FVector2D& Position, const FVector2D& Target);

    void DrawDirection(const FVector2D& Position, const FVector2D& Direction);

    void DrawVelocity(const FVector2D& Position, const FVector2D& Velocity);

    void DrawSteering(const FVector2D& Position, const FVector2D& Steering);

    FMassEntityQuery SteeringEntityQuery;
    FMassEntityQuery OctreeEntityQuery;

    USimTimeAuthoritySubsystem* SimTimeAuthoritySys;

    URandomAuthoritySys* RandomAuthoritySys;

    UOctreeSys* OctreeSys;

    uint32 LastTickCalculated = 21;

    const int32 SEEK_WEIGHT = 1;
    const int32 SEPARATION_WEIGHT = 2;
    const int32 AVOID_WEIGHT = 5;
};
