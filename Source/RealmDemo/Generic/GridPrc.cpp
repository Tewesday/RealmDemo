


#include "GridPrc.h"

void UGridPrc::Initialize(UObject& Owner)
{
    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    ExecutionOrder.ExecuteAfter.Add(TEXT("SteeringPrc"));
    ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UGridPrc::ConfigureQueries()
{
    GridEntityQuery.AddRequirement<FMGridFragment>(EMassFragmentAccess::ReadWrite);
    GridEntityQuery.RegisterWithProcessor(*this);
}

void UGridPrc::Execute(FMassEntityManager& MassEntityManager, FMassExecutionContext& Context)
{
    if (SimTimeAuthoritySys == nullptr) {
        SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
    }

    if (GridSys == nullptr) {
        GridSys = GetWorld()->GetSubsystem<UGridSys>();
        //GridSys->DrawGrid(GetWorld(), true);
    }

    GridEntityQuery.ForEachEntityChunk(MassEntityManager, Context,
    [&, this](FMassExecutionContext& Context) {
        const TArrayView<FMGridFragment> EntityGridFragments = Context.GetMutableFragmentView<FMGridFragment>();

        // Non Tick Based processing

        // Tick Based processing

        if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
            return;
        }

        const int32 QueryLength = Context.GetNumEntities();

        for (int32 i = 0; i < QueryLength; ++i) {


        }
    });
}