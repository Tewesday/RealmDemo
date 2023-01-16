


#include "OrderCleanupPrc.h"


void UOrderCleanupPrc::Initialize(UObject& Owner)
{
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	ExecutionOrder.ExecuteAfter.Add(TEXT("OrderPrc"));
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UOrderCleanupPrc::ConfigureQueries()
{
	OrderEntityQuery.AddRequirement<FMOrderFragment>(EMassFragmentAccess::ReadWrite);
	OrderEntityQuery.RegisterWithProcessor(*this);
}

void UOrderCleanupPrc::Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context)
{
	if (SimTimeAuthoritySys == nullptr) {
		SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
	}

	if (OrderSys == nullptr) {
		OrderSys = GetWorld()->GetSubsystem<UOrderSys>();
	}


	OrderEntityQuery.ForEachEntityChunk(EntitySubsystem, Context,
		[&, this](FMassExecutionContext& Context) {
			const TArrayView<FMOrderFragment> EntityOrderFragments = Context.GetMutableFragmentView<FMOrderFragment>();

			// Non Tick Based processing

			// Tick Based processing

			if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
				return;
			}

			const int32 QueryLength = Context.GetNumEntities();

			for (int32 i = 0; i < QueryLength; ++i) {

			}
		});

	OrderSys->CleanupOrderMessages();

	LastTickCalculated = SimTimeAuthoritySys->SimTimeAuthority.CurrentTick;
}