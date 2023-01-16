

#include "SteeringPrc.h"

USteeringPrc::USteeringPrc()
{
    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void USteeringPrc::Initialize(UObject& Owner)
{
    // Access violation, not ready yet?
    // SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
    
}

void USteeringPrc::ConfigureQueries()
{
    SteeringEntityQuery.AddRequirement<FMSteeringFragment>(EMassFragmentAccess::ReadWrite);
    SteeringEntityQuery.AddRequirement<FMOrderFragment>(EMassFragmentAccess::ReadWrite);
    SteeringEntityQuery.RegisterWithProcessor(*this);

    OctreeEntityQuery.AddRequirement<FMSteeringFragment>(EMassFragmentAccess::ReadWrite);
    OctreeEntityQuery.AddRequirement<FMOctreeFragment>(EMassFragmentAccess::ReadWrite);
    OctreeEntityQuery.RegisterWithProcessor(*this);
    
    //OrderEntityQuery.AddRequirement<FMSteeringFragment>(EMassFragmentAccess::ReadOnly);
    //OrderEntityQuery.RegisterWithProcessor(*this);
}

void USteeringPrc::Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context)
{
    if (SimTimeAuthoritySys == nullptr) {
        SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
    }

    if (RandomAuthoritySys == nullptr) {
        RandomAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<URandomAuthoritySys>();
        RandomAuthoritySys->Random.GenerateNewSeed();
    }
    
    if (OctreeSys == nullptr) {
        OctreeSys = GetWorld()->GetGameInstance()->GetSubsystem<UOctreeSys>();
    }

    SteeringEntityQuery.ForEachEntityChunk(EntitySubsystem, Context,
        [&, this](FMassExecutionContext& Context) {
            const TArrayView<FMSteeringFragment> EntitySteeringFragments = Context.GetMutableFragmentView<FMSteeringFragment>();
            const TArrayView<FMOrderFragment> EntityOrderFragments = Context.GetMutableFragmentView<FMOrderFragment>();

            // Non Tick Based processing

            // Tick Based processing

            if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
                return;
            }

            const int32 QueryLength = Context.GetNumEntities();

            for (int32 i = 0; i < QueryLength; ++i) {

                // Update Steering to match Order if needed
                if (EntityOrderFragments[i].OrderNeedsUpdate) {

                    // TODO: Override vs Add
                    if (EntityOrderFragments[i].Order == EMoveOrderType::MoveTo) {
                        EntitySteeringFragments[i].WaypointGoals.Empty();
                        EntitySteeringFragments[i].WaypointGoals.Add(EntityOrderFragments[i].OrderTargetPosition);
                        EntitySteeringFragments[i].Target = EntityOrderFragments[i].OrderTargetPosition;
                        //EntitySteeringFragments[i].Target = EntitySteeringFragments[i].WaypointGoals[0];
                    }
                    else if (EntityOrderFragments[i].Order == EMoveOrderType::Chase) {
                        //EntitySteeringFragments[i].Target = GetTargetBasedOnEntityTarget(EntitySubsystem, EntityOrderFragments[i].OrderTargetEntity);
                        EntitySteeringFragments[i].WaypointGoals.Add(GetTargetBasedOnEntityTarget(EntitySubsystem, EntityOrderFragments[i].OrderTargetEntity));
                    }
                    
                    EntityOrderFragments[i].OrderNeedsUpdate = false;
                }

                // Move each entity according to steering rules
                if (EntityOrderFragments[i].Order == EMoveOrderType::Idle) {

                    // It's too expensive to call this on all idle entities.
                    //FVector2D Steering = CalculateAvoidanceAroundSelf(Context.GetEntity(i), EntitySteeringFragments[i]);
                    //if (Steering != FVector2D::ZeroVector) {
                    //    UpdatePosition(Steering, EntitySteeringFragments[i]);
                    //}
                    continue;
                }

                if (EntityOrderFragments[i].Order == EMoveOrderType::Chase) {
                    // TODO: Test formations
                    EntitySteeringFragments[i].Target = GetTargetBasedOnEntityTarget(EntitySubsystem, EntityOrderFragments[i].OrderTargetEntity);
                }

                if (EntityOrderFragments[i].Order == EMoveOrderType::MoveTo) {

                }

                FVector2D Steering = CalculateSteeringTowardsTarget(EntitySteeringFragments[i].Target, EntitySteeringFragments[i]);
                Steering += CalculateAvoidanceAroundSelf(Context.GetEntity(i), EntitySteeringFragments[i]);

                //DrawVelocity(EntitySteeringFragments[i].Position, EntitySteeringFragments[i].Velocity);
                auto Direction = GetAndUpdateFacingDirection(EntitySteeringFragments[i]);
                //DrawDirection(EntitySteeringFragments[i].Position, Direction);

                UpdatePosition(Steering, EntitySteeringFragments[i]);
                if (IsDestinationReached(EntitySteeringFragments[i])) {

                    // TODO: Move to function

                    if (!EntitySteeringFragments[i].ControlledByFormation) {
                        if (EntitySteeringFragments[i].WaypointGoals.Num() > 0) {

                            // If the first WaypointGoal has been reached
                            if (EntitySteeringFragments[i].Target.Equals(EntitySteeringFragments[i].WaypointGoals[0])) {
                                EntitySteeringFragments[i].WaypointGoals.Pop();
                            }

                            // If there is another WaypointGoal
                            if (EntitySteeringFragments[i].WaypointGoals.Num() > 0) {
                                EntitySteeringFragments[i].Target = EntitySteeringFragments[i].WaypointGoals[0];
                            }
                            else {
                                EntityOrderFragments[i].Order = EMoveOrderType::Idle;
                            }


                        }
                        else {
                            EntityOrderFragments[i].Order = EMoveOrderType::Idle;
                        }
                    }
                    
                    //FVector2D RandomTarget = GetARandomPosition(RandomAuthoirtySys->Random, -900.f, 900.f);
                    //UpdateTarget(RandomTarget, EntitySteeringFragments[i]);
                }
            }
        });

    OctreeEntityQuery.ForEachEntityChunk(EntitySubsystem, Context,
        [&, this](FMassExecutionContext& Context) {
            const TArrayView<FMSteeringFragment> EntitySteeringFragments = Context.GetMutableFragmentView<FMSteeringFragment>();
            const TArrayView<FMOctreeFragment> EntityOctreeFragments = Context.GetMutableFragmentView<FMOctreeFragment>();

            // Non Tick Based processing

            // Tick Based processing

            if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
                return;
            }

            const int32 QueryLength = Context.GetNumEntities();

            for (int32 i = 0; i < QueryLength; ++i) {
                EntityOctreeFragments[i].OctreeNeedsUpdate = EntitySteeringFragments[i].NeedsOctreeUpdate;
                EntitySteeringFragments[i].NeedsOctreeUpdate = false;
            }
    });


    LastTickCalculated = SimTimeAuthoritySys->SimTimeAuthority.CurrentTick;
}

void USteeringPrc::UpdateTarget(FVector2D Target, FMSteeringFragment& SteeringFragment)
{
    SteeringFragment.Target = Target;
}

FVector2D USteeringPrc::CalculateSteeringTowardsTarget(FVector2D Target, FMSteeringFragment& SteeringFragment)
{
    // Find Seek Position, this ends up being = to Target, may be unnecessary
    FVector2D DesiredPosition = Target - SteeringFragment.Position;
    DesiredPosition.Normalize();

    FVector2D DesiredVelocity = FVector2D(DesiredPosition.X * SteeringFragment.MaxVelocity.X, DesiredPosition.Y * SteeringFragment.MaxVelocity.Y);

    // Arrival Force
    if (SteeringFragment.MaxVelocity.X > UE::Geometry::DistanceSquared(SteeringFragment.Position, SteeringFragment.Target)) {
        DesiredVelocity = DesiredVelocity / SteeringFragment.MaxVelocity;
    }

    FVector2D Steering = (DesiredVelocity - SteeringFragment.Velocity) * SEEK_WEIGHT;
    return Steering;
}

FVector2D USteeringPrc::CalculateAvoidanceAroundSelf(const FMassEntityHandle SelfHandle, FMSteeringFragment& SteeringFragment)
{
    // TODO: Improve
    if (SteeringFragment.ControlledByFormation) {
        return FVector2D::ZeroVector;
    }

    FVector2D Steering = FVector2D::ZeroVector;

    float MaxAvoidForce = SteeringFragment.MaxVelocity.X * 0.80f;
    int32 RelevantObstacleNum = 0;

    // Get adjacent obstacles from Octree
    // TODO: USE ENTITY RADIUS
    float radius = 25;

    //std::vector<OrthoTree::entity_id_type> IDs = FindEntitiesAroundSelfInRadius(SteeringFragment.Position, radius);
    std::vector<OrthoTree::entity_id_type> IDs = FindEntitiesAroundSelf(SteeringFragment);
    //std::vector<OrthoTree::entity_id_type> IDs = FindNearestEntitiesAroundSelfPoint(SteeringFragment);
    //TArray<OrthoTree::entity_id_type> IDs;
    //OctreeSys->TreeIDToEntityHandleMap.GetKeys(IDs);

    FVector2D TotalForce = FVector2D::ZeroVector;

    TArray<FVector> ObstaclePositions;

    UEntityActorSys* EntityActorSys = GetWorld()->GetSubsystem<UEntityActorSys>();

    DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Avoidance"), STAT_Avoidance, STATGROUP_AVOIDANCE);
    for (int32 IDsIndex = 0; IDsIndex < IDs.size(); ++IDsIndex) {
    //for (int32 IDsIndex = 0; IDsIndex < IDs.Num(); ++IDsIndex) {
        FMassEntityHandle Entity = OctreeSys->TreeIDToEntityHandleMap[IDs[IDsIndex]];
        //FMassEntityHandle Entity = OctreeSys->PointTreeIDToEntityHandleMap[IDs[IDsIndex]];
        // Skip self
        if (Entity == SelfHandle) {
            continue;
        }

        FMassEntityView EntityView = EntityActorSys->GetEntityView(Entity);
        auto* EntitySteeringFragment = EntityView.GetFragmentDataPtr<FMSteeringFragment>();
        
        float Distance = UE::Geometry::DistanceSquared(SteeringFragment.Position, EntitySteeringFragment->Position);

        if (Distance <= (radius * 3) * (radius * 3)) {
            FVector2D SeparationForce = (SteeringFragment.Position - (EntitySteeringFragment->Position) + FVector2D(SMALL_NUMBER));
            ObstaclePositions.Add(FVector(SeparationForce, 0.f));
        }
    }

    const auto AverageRepulsion = UKismetMathLibrary::GetVectorArrayAverage(ObstaclePositions);

    return FVector2D(AverageRepulsion.X, AverageRepulsion.Y) * SEPARATION_WEIGHT;
}

std::vector<OrthoTree::entity_id_type> USteeringPrc::FindEntitiesAroundSelfInRadius(FVector2D Position, float Radius)
{
    DECLARE_SCOPE_CYCLE_COUNTER(TEXT("FindEntities"), STAT_FindEntities, STATGROUP_FINDENTITIES);
    auto SearchBox = OrthoTree::BoundingBox2D({ Position.X - Radius, Position.Y - Radius }, { Position.X + Radius, Position.Y + Radius });
    //std::vector<OrthoTree::entity_id_type> IDs = OctreeSys->QuadTree.RangeSearch<false /*overlap allowed*/>(SearchBox);
    return OctreeSys->QuadTree.RangeSearch<false /*overlap allowed*/>(SearchBox);
}

std::vector<OrthoTree::entity_id_type> USteeringPrc::FindEntitiesAroundSelf(FMSteeringFragment& SteeringFragment)
{
    DECLARE_SCOPE_CYCLE_COUNTER(TEXT("FindEntities"), STAT_FindEntities, STATGROUP_FINDENTITIES);
    //auto SearchBox = OrthoTree::BoundingBox2D({ Position.X - Radius, Position.Y - Radius }, { Position.X + Radius, Position.Y + Radius });
    //std::vector<OrthoTree::entity_id_type> IDs = OctreeSys->QuadTree.RangeSearch<false /*overlap allowed*/>(SearchBox);
    return OctreeSys->QuadTree.RangeSearch<false /*overlap allowed*/>(SteeringFragment.BoundingBox);
}

std::vector<OrthoTree::entity_id_type> USteeringPrc::FindNearestEntitiesAroundSelfPoint(FMSteeringFragment& SteeringFragment) 
{
    DECLARE_SCOPE_CYCLE_COUNTER(TEXT("FindEntities"), STAT_FindEntities, STATGROUP_FINDENTITIES);
    return OctreeSys->PointTree.GetNearestNeighbors(OrthoTree::Point3D{ SteeringFragment.Position.X, SteeringFragment.Position.Y }, 8);
}

void USteeringPrc::UpdatePosition(FVector2D Steering, FMSteeringFragment& SteeringFragment)
{
    // Apply Steering, within minimum and maximum movement speed
    SteeringFragment.Velocity = ((SteeringFragment.Velocity + Steering)).ClampAxes(-SteeringFragment.MaxVelocity.X, SteeringFragment.MaxVelocity.X);

    // Update Position
    SteeringFragment.Position = SteeringFragment.Position + SteeringFragment.Velocity;

    // TODO: USE ENTITY RADIUS
    const float Radius = 25;
    SteeringFragment.BoundingBox = OrthoTree::BoundingBox2D({ SteeringFragment.Position.X - Radius, SteeringFragment.Position.Y - Radius }, { SteeringFragment.Position.X + Radius, SteeringFragment.Position.Y + Radius });

    SteeringFragment.NeedsOctreeUpdate = true;
}

bool USteeringPrc::IsDestinationReached(FMSteeringFragment& SteeringFragment)
{
    return (1 > UE::Geometry::DistanceSquared(SteeringFragment.Position, SteeringFragment.Target));
}

FVector2D USteeringPrc::GetARandomPosition(FRandomStream& RandomStream, double Min, double Max)
{
    auto RandomFloatX = RandomStream.FRandRange(Min, Max);
    auto RandomFloatY = RandomStream.FRandRange(Min, Max);

    return FVector2D(RandomFloatX, RandomFloatY);
}

FVector2D USteeringPrc::GetTargetBasedOnEntityTarget(FMassEntityManager& EntityManager, FMassEntityHandle EntityTarget)
{
    FMassEntityView EntityView = FMassEntityView(EntityManager, EntityTarget);

    FMSteeringFragment* TargetSteeringFragment = EntityView.GetFragmentDataPtr<FMSteeringFragment>();
    
    return TargetSteeringFragment->Position;
}

FVector2D USteeringPrc::GetAndUpdateFacingDirection(FMSteeringFragment& SteeringFragment)
{
    FVector2D Direction;

    // Calc vector from position to target
    Direction = SteeringFragment.Target - SteeringFragment.Position;

    // Shrink to direction (unit) vector
    Direction.Normalize();

    SteeringFragment.Direction = Direction;

    return Direction;
}

void USteeringPrc::DrawDirection(const FVector2D& Position, const FVector2D& Direction)
{
    const float LineHeight = 100.f;
    DrawDebugLine(GetWorld(), FVector(Position, LineHeight), FVector(Direction + Position, LineHeight), FColor::MakeRandomColor(), false, 0.1f);
}

void USteeringPrc::DrawVelocity(const FVector2D& Position, const FVector2D& Velocity)
{
    const float LineHeight = 100.f;
    auto Color = FColor::MakeRandomColor();
    DrawDebugLine(GetWorld(), FVector(Position, LineHeight), FVector(Velocity + Position, LineHeight), Color, false, 0.1f);
}

void USteeringPrc::DrawSteering(const FVector2D& Position, const FVector2D& Velocity)
{
    const float LineHeight = 100.f;
    DrawDebugLine(GetWorld(), FVector(Position, LineHeight), FVector(Velocity + Position, LineHeight), FColor::MakeRandomColor(), false, 0.1f);
}

FVector2D USteeringPrc::GetFacingDirection(const FVector2D& Position, const FVector2D& Target) 
{
    FVector2D Direction;

    // Calc vector from position to target
    Direction = Target - Position;

    // Shrink to direction (unit) vector
    Direction.Normalize();

    return Direction;
}