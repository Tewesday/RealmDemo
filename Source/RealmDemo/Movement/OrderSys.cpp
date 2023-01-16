


#include "OrderSys.h"

void UOrderSys::InstructSelection(EOrderType OrderTypeTarget, FVector2D Location)
{
	USelectionSys* SelectionSubystem = GetWorld()->GetGameInstance()->GetSubsystem<USelectionSys>();

	TArray<FMassEntityHandle> Selection = SelectionSubystem->CurrentlySelectedEntities;
	for (int32 SelectionIndex = 0; SelectionIndex < Selection.Num(); ++SelectionIndex) {
		AddOrderMessage(SelectionSubystem->CurrentlySelectedEntities[SelectionIndex], OrderTypeTarget, Location);
	}
}

void UOrderSys::InstructSelectionWithTarget(EOrderType OrderTypeTarget, FMassEntityHandle EntityTarget)
{
	USelectionSys* SelectionSubystem = GetWorld()->GetGameInstance()->GetSubsystem<USelectionSys>();

	TArray<FMassEntityHandle> Selection = SelectionSubystem->CurrentlySelectedEntities;
	for (int32 SelectionIndex = 0; SelectionIndex < Selection.Num(); ++SelectionIndex) {
		AddOrderWithTargetMessage(SelectionSubystem->CurrentlySelectedEntities[SelectionIndex], EOrderType::Attack, EntityTarget);
	}
}

void UOrderSys::AddOrderMessage(FMassEntityHandle Entity, EOrderType OrderTypeTarget, FVector2D Location)
{
	FOrderEventMessage OrderMessage = FOrderEventMessage();
	OrderMessage.EntityTarget = Entity;
	OrderMessage.OrderTypeTarget = OrderTypeTarget;
	OrderMessage.OrderTargetLocation = Location;

	OrderEventMessages.Add(OrderMessage);
}

void UOrderSys::AddOrderWithTargetMessage(FMassEntityHandle Entity, EOrderType OrderTypeTarget, FMassEntityHandle EntityTarget)
{
	FOrderEventMessage OrderMessage = FOrderEventMessage();
	OrderMessage.EntityTarget = Entity;
	OrderMessage.OrderTypeTarget = OrderTypeTarget;
	OrderMessage.OrderEntityTarget = EntityTarget;

	OrderEventMessages.Add(OrderMessage);
}

void UOrderSys::RemoveOrderMessage(int Index)
{
	OrderEventMessages.RemoveAt(Index);
}

void UOrderSys::MarkOrderMessageForDeletion(int32 TargetIndex)
{
	OrderEventMessages[TargetIndex].MarkedForDeletion = true;
}

void UOrderSys::CleanupOrderMessages()
{
	int32 Count = OrderEventMessages.Num();
	for (int32 i = (Count - 1); i > 0; --i) {
		if (OrderEventMessages[i].MarkedForDeletion) {
			RemoveOrderMessage(i);
		}
	}
}
