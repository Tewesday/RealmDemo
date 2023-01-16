


#include "SelectionSys.h"


void USelectionSys::AddModifySelectionMessage(TArray<FMassEntityHandle> Entities)
{
	FModifySelectionEventMessage Message = FModifySelectionEventMessage();
	Message.EntityTargets.Append(Entities);

	ModifySelectionMessages.Add(Message);
}

void USelectionSys::AddModifySelectionWithIdMessage(TArray<FMassEntityHandle> Entities, int32 SelectionId)
{
	FModifySelectionEventMessage Message = FModifySelectionEventMessage();
	Message.EntityTargets.Append(Entities);
	Message.SelectionId = SelectionId;

	ModifySelectionMessages.Add(Message);
}

void USelectionSys::RemoveModifySelectionMessage(int Index)
{
	ModifySelectionMessages.RemoveAt(Index);
}

void USelectionSys::AddSelectionMessage()
{
	FSelectionEventMessage Message = FSelectionEventMessage();
	Message.SelectionId = -1;

	SelectionMessages.Add(Message);
}

void USelectionSys::AddSelectionWithIdMessage(int32 SelectionId)
{
	FSelectionEventMessage Message = FSelectionEventMessage();
	Message.SelectionId = SelectionId;

	SelectionMessages.Add(Message);
}

void USelectionSys::RemoveSelectionMessage(int Index)
{
	SelectionMessages.RemoveAt(Index);
}

void USelectionSys::MarkModifySelectionMessageForDeletion(int32 TargetIndex)
{
	ModifySelectionMessages[TargetIndex].MarkedForDeletion = true;
}

void USelectionSys::MarkSelectionMessageForDeletion(int32 TargetIndex)
{
	SelectionMessages[TargetIndex].MarkedForDeletion = true;
}