


#include "OrderPrc.h"

void UOrderPrc::Initialize(UObject& Owner)
{
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	ExecutionOrder.ExecuteBefore.Add(TEXT("SteeringPrc"));
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UOrderPrc::ConfigureQueries()
{
	OrderEntityQuery.AddRequirement<FMOrderFragment>(EMassFragmentAccess::ReadWrite);
	OrderEntityQuery.AddRequirement<FMSteeringFragment>(EMassFragmentAccess::ReadOnly);
	OrderEntityQuery.RegisterWithProcessor(*this);
}

void UOrderPrc::Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context)
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
            const TConstArrayView<FMSteeringFragment> EntitySteeringFragments = Context.GetFragmentView<FMSteeringFragment>();

            // Non Tick Based processing

            // Tick Based processing

            if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
                return;
            }

            const int32 QueryLength = Context.GetNumEntities();

            for (int32 i = 0; i < QueryLength; ++i) {
				FMassEntityHandle LocalEntityHandle = Context.GetEntity(i);
				int32 OrderMessagesCount = OrderSys->OrderEventMessages.Num();

				if (OrderMessagesCount > 0) {
					//FOrderEventMessage OrderMessage = GetLatestOrderForEntity(LocalEntityHandle);

					// Continue looping so that Order is most up to date
					for (int32 OrderMessagesIndex = 0; OrderMessagesIndex < OrderMessagesCount; ++OrderMessagesIndex) {
						FOrderEventMessage OrderMessage = OrderSys->OrderEventMessages[OrderMessagesIndex];
						if (OrderMessage.MarkedForDeletion) {
							continue;
						}

						if (OrderMessage.EntityTarget == LocalEntityHandle) {

							EntityOrderFragments[i].Order = ConvertOrderTypeToMoveOrderType(OrderMessage.OrderTypeTarget);

							if (EntitySubsystem.IsEntityValid(OrderMessage.OrderEntityTarget)) {
								EntityOrderFragments[i].OrderTargetEntity = OrderMessage.OrderEntityTarget;
							}
							else {
								EntityOrderFragments[i].OrderTargetPosition = OrderMessage.OrderTargetLocation;
							}

							EntityOrderFragments[i].OrderNeedsUpdate = true;

							// Message will be deleted by another processor
							OrderSys->MarkOrderMessageForDeletion(OrderMessagesIndex);
						}

					}

					//if (!(OrderMessage.EntityTarget == LocalEntityHandle)) {
					//	continue;
					//}

					//EntityOrderFragments[i].Order = ConvertOrderTypeToMoveOrderType(OrderMessage.OrderTypeTarget);

					//if (EntitySubsystem.IsEntityValid(OrderMessage.OrderEntityTarget)) {
					//	EntityOrderFragments[i].OrderTargetEntity = OrderMessage.OrderEntityTarget;
					//}
					//else {
					//	EntityOrderFragments[i].OrderTargetPosition = OrderMessage.OrderTargetLocation;
					//}

					//EntityOrderFragments[i].OrderNeedsUpdate = true;
				}

            }
        });

    LastTickCalculated = SimTimeAuthoritySys->SimTimeAuthority.CurrentTick;
}

FOrderEventMessage UOrderPrc::GetLatestOrderForEntity(FMassEntityHandle EntityHandle)
{
	FOrderEventMessage LatestOrderMessage;

	int32 OrderMessagesCount = OrderSys->OrderEventMessages.Num();

	// See if any relevant OrderEventMessages exist
	if (OrderMessagesCount > 0) {

		// Continue looping so that Order is most up to date
		for (int32 OrderMessagesIndex = 0; OrderMessagesIndex < OrderMessagesCount; ++OrderMessagesIndex) {
			FOrderEventMessage OrderMessage = OrderSys->OrderEventMessages[OrderMessagesIndex];

			if (OrderMessage.EntityTarget == EntityHandle) {
				LatestOrderMessage = OrderMessage;

				// Message will be deleted by another processor
				OrderSys->MarkOrderMessageForDeletion(OrderMessagesIndex);
			}

		}
	}
    return LatestOrderMessage;
}

EMoveOrderType UOrderPrc::ConvertOrderTypeToMoveOrderType(EOrderType OrderType)
{
	switch (OrderType) {
		case EOrderType::Idle: {
			return EMoveOrderType::Idle;
		}
		case EOrderType::MoveTo: {
			return EMoveOrderType::MoveTo;
		}
		case EOrderType::Attack: {
			return EMoveOrderType::Chase;
		}
	}

	return EMoveOrderType::Idle;
}
