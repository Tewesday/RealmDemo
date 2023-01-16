


#include "FormationPrc.h"

void UFormationPrc::Initialize(UObject& Owner)
{
    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    ExecutionOrder.ExecuteAfter.Add(TEXT("SteeringPrc"));
    ExecutionFlags = (int32)EProcessorExecutionFlags::All;

    UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
    FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();

    FormationEntityArchetype = EntityManager->CreateArchetype(TConstArrayView<const UScriptStruct*> {
        FMFormationFragment::StaticStruct(),
    });
}

void UFormationPrc::ConfigureQueries()
{
    FormationMemberEntityQuery.AddRequirement<FMFormationMemberFragment>(EMassFragmentAccess::ReadWrite);
    FormationMemberEntityQuery.RegisterWithProcessor(*this);

    FormationEntityQuery.AddRequirement<FMFormationFragment>(EMassFragmentAccess::ReadWrite);
    FormationEntityQuery.RegisterWithProcessor(*this);
}

void UFormationPrc::Execute(FMassEntityManager& MassEntityManager, FMassExecutionContext& Context)
{
    if (SimTimeAuthoritySys == nullptr) {
        SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
    }

    if (FormationSys == nullptr) {
        FormationSys = GetWorld()->GetSubsystem<UFormationSys>();

        FMassEntityHandle Entity = MassEntityManager.CreateEntity(FormationEntityArchetype);

        MassEntityManager.CheckIfEntityIsValid(Entity);
        MassEntityManager.CheckIfEntityIsActive(Entity);

        FormationSys->FormationHandles.Add(Entity);
    }

    FormationMemberEntityQuery.ForEachEntityChunk(MassEntityManager, Context,
    [&, this](FMassExecutionContext& Context) {
        const TArrayView<FMFormationMemberFragment> EntityFormationMemberFragments = Context.GetMutableFragmentView<FMFormationMemberFragment>();

        // Non Tick Based processing

        // Tick Based processing

        if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
            return;
        }

        const int32 QueryLength = Context.GetNumEntities();

        FMassEntityHandle FormationHandle;


        if (FormationSys->FormationHandles.Num() > 0) {
            FormationHandle = FormationSys->FormationHandles[0];
            FMassEntityView EntityView = FMassEntityView(MassEntityManager, FormationHandle);
            auto* FormationFragment = EntityView.GetFragmentDataPtr<FMFormationFragment>();

            //// TODO: FOR NOW add all to formation
            //for (int32 i = 0; i < QueryLength; ++i) {
            //    if (EntityFormationMemberFragments[i].InFormation) {
            //        continue;
            //    }
            //    FormationFragment->EntitiesRequestingToJoin.Add(Context.GetEntity(i));
            //}
        }

        //for (int32 i = 0; i < QueryLength; ++i) {
        //    
        //}
    });

    FormationEntityQuery.ForEachEntityChunk(MassEntityManager, Context,
    [&, this](FMassExecutionContext& Context) {
        const TArrayView<FMFormationFragment> EntityFormationFragments = Context.GetMutableFragmentView<FMFormationFragment>();

        // Non Tick Based processing

        // Tick Based processing

        if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
            return;
        }

        const int32 QueryLength = Context.GetNumEntities();

        for (int32 i = 0; i < QueryLength; ++i) {

            //if (EntityFormationFragments[i].Target == EntityFormationFragments[i].DEFAULT_TARGET) {
            //    EntityFormationFragments[i].Target = FVector2D(1000.f, 1000.f);
            //}

            ////
            //float Distance = UE::Geometry::DistanceSquared(EntityFormationFragments[i].Position, EntityFormationFragments[i].Target);

            //if (Distance < 1.f) {
            //    continue;
            //}

            // TODO: Use avoidance between formations
            //AddMembersToFormation(EntityFormationFragments[i]);
            //UpdateFormationPosition(EntityFormationFragments[i]);
            //ApplyPositionsToEntitiesInFormation(EntityFormationFragments[i]);
        }
    });

    LastTickCalculated = SimTimeAuthoritySys->SimTimeAuthority.CurrentTick;
}

void UFormationPrc::AddMembersToFormation(FMFormationFragment& Formation)
{
    uint32 EntityReqCount = Formation.EntitiesRequestingToJoin.Num();
    uint32 EntityCount = Formation.EntitiesInFormation.Num();

    UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
    FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();


    if (EntityReqCount > 0) {
        int32 Total = (EntityReqCount + EntityCount);

        int32 SqRtOfTotal = sqrt(Total);
        
        Formation.UnitRadius = Formation.UnitRadius * 3;
        Formation.FormationGrid = Grid(SqRtOfTotal + 1, SqRtOfTotal, Formation.UnitRadius);
        Formation.FormationGrid.GenerateGrid();
    }
    else {
        return;
    }
    
    TArray<std::tuple<FMassEntityHandle, uint32, FVector2D>> Members;

    // Add current entities in formation to sort array
    for (uint32 MemberIndex = 0; MemberIndex < EntityCount; ++MemberIndex) {
        FMassEntityHandle Entity = Formation.EntitiesInFormation[MemberIndex];
        FMassEntityView EntityView = FMassEntityView(*EntityManager, Entity);
        auto* FormationMemberFragment = EntityView.GetFragmentDataPtr<FMFormationMemberFragment>();
        uint32 FormationIndex = FormationMemberFragment->FormationIndex;

        auto* MemberSteeringFragment = EntityView.GetFragmentDataPtr<FMSteeringFragment>();

        Members.Add(std::make_tuple(Entity, FormationIndex, MemberSteeringFragment->Position));
    }

    // Add new entities to formation and to sort array
    for (uint32 MemberIndex = 0; MemberIndex < EntityReqCount; ++MemberIndex) {
        FMassEntityHandle Entity = Formation.EntitiesRequestingToJoin[MemberIndex];
        FMassEntityView EntityView = FMassEntityView(*EntityManager, Entity);
        auto* FormationMemberFragment = EntityView.GetFragmentDataPtr<FMFormationMemberFragment>();

        FormationMemberFragment->FormationIndex = EntityCount + MemberIndex;
        FormationMemberFragment->InFormation = true;
        Formation.EntitiesInFormation.Add(Entity);

        auto* MemberSteeringFragment = EntityView.GetFragmentDataPtr<FMSteeringFragment>();

        MemberSteeringFragment->ControlledByFormation = true;

        // Update formation position to match first unit in formation
        if (EntityCount == 0 && MemberIndex == 0) {
            Formation.Position = MemberSteeringFragment->Position;
            Formation.FormationGrid.SetGridWorldPosition(Formation.Position);
        }

        Members.Add(std::make_tuple(Entity, FormationMemberFragment->FormationIndex, MemberSteeringFragment->Position));
    }

    Formation.EntitiesRequestingToJoin.Empty();

    // Sort Members by distance from Formation position
    Members.Sort([Formation](auto const& t1, auto const& t2) {
        const FVector2D MemberPositionA = std::get<FVector2D>(t1);
        const FVector2D MemberPositionB = std::get<FVector2D>(t2);

        return FVector2D::Distance(MemberPositionA, Formation.Position) < FVector2D::Distance(MemberPositionB, Formation.Position);
    });

    // Apply sort results to Formation
    Formation.EntitiesInFormation.Empty();

    for (int32 MemberIndex = 0; MemberIndex < Members.Num(); ++MemberIndex) {
        FMassEntityHandle Entity = std::get<FMassEntityHandle>(Members[MemberIndex]);
        FMassEntityView EntityView = FMassEntityView(*EntityManager, Entity);
        auto* FormationMemberFragment = EntityView.GetFragmentDataPtr<FMFormationMemberFragment>();
        FormationMemberFragment->FormationIndex = MemberIndex;

        auto& GridPosition = Formation.FormationGrid.TileGridPositions[MemberIndex];
        FormationMemberFragment->OffsetTarget = Formation.FormationGrid.GetHexWorldPosition(GridPosition);

        Formation.EntitiesInFormation.Add(Entity);
    }
}

void UFormationPrc::UpdateFormationPosition(FMFormationFragment& Formation)
{
    // Find Seek Position, this ends up being = to Target, may be unnecessary
    FVector2D DesiredPosition = Formation.Target - Formation.Position;
    DesiredPosition.Normalize();

    FVector2D DesiredVelocity = FVector2D(DesiredPosition.X * Formation.MaxVelocity.X, DesiredPosition.Y * Formation.MaxVelocity.Y);

    // Arrival Force
    if (Formation.MaxVelocity.X > UE::Geometry::DistanceSquared(Formation.Position, Formation.Target)) {
        DesiredVelocity = DesiredVelocity / Formation.MaxVelocity;
    }

    FVector2D Steering = (DesiredVelocity - Formation.Velocity) * SEEK_WEIGHT;

    // Apply Steering, within minimum and maximum movement speed
    Formation.Velocity = ((Formation.Velocity + Steering)).ClampAxes(-Formation.MaxVelocity.X, Formation.MaxVelocity.X);

    // Store Position
    Formation.LastPosition = Formation.Position;

    // Update Position
    Formation.Position = Formation.Position + Formation.Velocity;

    Formation.FromLastToCurrentPos = Formation.Position - Formation.LastPosition;
    //return Steering;
}

void UFormationPrc::ApplyPositionsToEntitiesInFormation(FMFormationFragment& Formation)
{
    UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
    FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();
    
    for (int32 MemberIndex = 0; MemberIndex < Formation.EntitiesInFormation.Num(); ++MemberIndex) {
        FMassEntityHandle Entity = Formation.EntitiesInFormation[MemberIndex];
        FMassEntityView EntityView = FMassEntityView(*EntityManager, Entity);
        auto* FormationMemberFragment = EntityView.GetFragmentDataPtr<FMFormationMemberFragment>();

        auto* MemberSteeringFragment = EntityView.GetFragmentDataPtr<FMSteeringFragment>();

        FormationMemberFragment->OffsetTarget = FormationMemberFragment->OffsetTarget + Formation.FromLastToCurrentPos;
        FVector2D TowardsDesiredPosition = FormationMemberFragment->OffsetTarget - MemberSteeringFragment->Position;

        MemberSteeringFragment->Target = MemberSteeringFragment->Position + TowardsDesiredPosition;

        // TODO: Should not be done this way (the entity should already be moving)
        auto* MemberOrderFragment = EntityView.GetFragmentDataPtr<FMOrderFragment>();
        MemberOrderFragment->Order = EMoveOrderType::MoveTo;
    }

}
