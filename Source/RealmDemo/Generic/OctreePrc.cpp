


#include "OctreePrc.h"

void UOctreePrc::Initialize(UObject& Owner)
{
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    bRequiresGameThreadExecution = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UOctreePrc::ConfigureQueries()
{
	OctreeQuery.AddRequirement<FMSteeringFragment>(EMassFragmentAccess::ReadOnly);
    OctreeQuery.AddRequirement<FMOctreeFragment>(EMassFragmentAccess::ReadWrite);
	OctreeQuery.RegisterWithProcessor(*this);
}

void UOctreePrc::Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context)
{
    FScopeLock lock(&criticalSection);

	if (SimTimeAuthoritySys == nullptr) {
        SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
    }

    if (OctreeSys == nullptr) {
        OctreeSys = GetWorld()->GetGameInstance()->GetSubsystem<UOctreeSys>();
    }

    if (EntityActorSys == nullptr) {
        EntityActorSys = GetWorld()->GetSubsystem<UEntityActorSys>();
    }

    OctreeQuery.ForEachEntityChunk(EntitySubsystem, Context,
    [&, this](FMassExecutionContext& Context) {
            const TConstArrayView<FMSteeringFragment> EntitySteeringFragments = Context.GetFragmentView<FMSteeringFragment>();
            const TArrayView<FMOctreeFragment> EntityOctreeFragments = Context.GetMutableFragmentView<FMOctreeFragment>();

            // Non Tick Based processing

            // Tick Based processing

            if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
                return;
            }

            //OctreeSys->VisualizeOctree();

            // TODO: USE ENTITY RADIUS
            float radius = 25;

            const int32 QueryLength = Context.GetNumEntities();

            for (int32 i = 0; i < QueryLength; ++i) {
                if (EntityOctreeFragments[i].NeedsFirstOctreeInsertion) {
                    DECLARE_SCOPE_CYCLE_COUNTER(TEXT("InsertOctree"), STAT_InsertOctree, STATGROUP_INSERTOCTREE);
                    FBoxCenterAndExtent BoundingCenterExtent = FBoxCenterAndExtent(FVector(EntitySteeringFragments[i].Position, 0.f), FVector(EntitySteeringFragments[i].Position.X + radius, EntitySteeringFragments[i].Position.Y + radius, 0));
                    UE::Geometry::FAxisAlignedBox3d BoundingBox = BoundingCenterExtent.GetBox();
                    
                    //OrthoTree::BoundingBox2D BoundingBox = OrthoTree::BoundingBox2D({ EntitySteeringFragments[i].Position.X - radius, EntitySteeringFragments[i].Position.Y - radius }, { EntitySteeringFragments[i].Position.X + radius, EntitySteeringFragments[i].Position.Y + radius });

                    // Insert box must overlap QuadTree bounds
                    
                    OctreeSys->QuadTree.InsertObject(OctreeSys->TreeIDs, BoundingBox);
                    
            
                    EntityOctreeFragments[i].OctreeID = OctreeSys->TreeIDs;

                    OctreeSys->TreeIDToEntityHandleMap.Add(OctreeSys->TreeIDs, Context.GetEntity(i));

                    //Inserted = OctreeSys->PointTree.Insert(OrthoTree::Point3D{ EntitySteeringFragments[i].Position.X, EntitySteeringFragments[i].Position.Y });

                    // if (std::get<bool>(Inserted)) {
                    //     OctreeSys->PointTreeIDToEntityHandleMap.Add(std::get<uint32>(Inserted), Context.GetEntity(i));
                    // }

                    EntityOctreeFragments[i].NeedsFirstOctreeInsertion = false;

                    // Parallel write?
                    ++OctreeSys->TreeIDs;
                }

                if (EntityOctreeFragments[i].OctreeNeedsUpdate) {
                    DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UpdateOctree"), STAT_UpdateOctree, STATGROUP_UPDATEOCTREE);
                    FBoxCenterAndExtent BoundingCenterExtent = FBoxCenterAndExtent(FVector(EntitySteeringFragments[i].Position, 0.f), FVector(EntitySteeringFragments[i].Position.X + radius, EntitySteeringFragments[i].Position.Y + radius, 0));
                    UE::Geometry::FAxisAlignedBox3d BoundingBox = BoundingCenterExtent.GetBox();
                    //OrthoTree::BoundingBox2D BoundingBox = OrthoTree::BoundingBox2D({ EntitySteeringFragments[i].Position.X - radius, EntitySteeringFragments[i].Position.Y - radius }, { EntitySteeringFragments[i].Position.X + radius, EntitySteeringFragments[i].Position.Y + radius });

                    bool Updated = OctreeSys->QuadTree.ReinsertObject(EntityOctreeFragments[i].OctreeID, BoundingBox);
                    if (Updated) {

                        //OctreeSys->PointTree.Update(EntityOctreeFragments[i].PointTreeID, OrthoTree::Point3D{ EntitySteeringFragments[i].Position.X, EntitySteeringFragments[i].Position.Y });

                        EntityOctreeFragments[i].OctreeNeedsUpdate = false;
                    }
                }
            }
    });

    LastTickCalculated = SimTimeAuthoritySys->SimTimeAuthority.CurrentTick;
}
