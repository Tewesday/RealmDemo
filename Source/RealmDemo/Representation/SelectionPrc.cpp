


#include "SelectionPrc.h"


void USelectionPrc::Initialize(UObject& Owner)
{
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	ExecutionOrder.ExecuteBefore.Add(TEXT("SteeringPrc"));
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;

	UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();
	SelectionEntityArchetype = EntityManager->CreateArchetype(TConstArrayView<const UScriptStruct*> {

	});
}

void USelectionPrc::ConfigureQueries()
{
	SelectionEntityQuery.AddRequirement<FSelectionFragment>(EMassFragmentAccess::ReadWrite);
	SelectionEntityQuery.RegisterWithProcessor(*this);
}

void USelectionPrc::Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context)
{
	if (SimTimeAuthoritySys == nullptr) {
		SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
	}

	if (SelectionSys == nullptr) {
		SelectionSys = GetWorld()->GetGameInstance()->GetSubsystem<USelectionSys>();
		//SelectionSys = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController()->GetLocalPlayer()->GetSubsystem<USelectionSys>();
		//SelectionSys = GetSubsystem<USelectionSys>();
		//SelectionSys = GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<USelectionSys>();
	}

	SelectionEntityQuery.ForEachEntityChunk(EntitySubsystem, Context, ([this](FMassExecutionContext& Context)
	{
		auto SelectionList = Context.GetMutableFragmentView<FSelectionFragment>();

		const float WorldDeltaTime = Context.GetDeltaTimeSeconds();

		// Non Tick Based processing

		// Tick Based processing

		if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
			return;
		}

		int32 ModifySelectionMessagesCount = SelectionSys->ModifySelectionMessages.Num();
		int32 SelectionMessagesCount = SelectionSys->SelectionMessages.Num();

		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			// Apply processor behavior to each entity

			// See if any relevant SelectionEventMessages exist
			if (ModifySelectionMessagesCount > 0) {
				auto LocalEntityHandle = Context.GetEntity(EntityIndex);

				// Continue looping so that Selection is most up to date
				for (int32 ModifySelectionMessagesIndex = 0; ModifySelectionMessagesIndex < ModifySelectionMessagesCount; ++ModifySelectionMessagesIndex) {
					auto SelectionMessage = SelectionSys->ModifySelectionMessages[ModifySelectionMessagesIndex];

					if (SelectionMessage.SelectionId == SelectionList[EntityIndex].SelectionId) {
						// Unload SelectionMessage
						if (SelectionMessage.ShouldAppend) {
							SelectionList[EntityIndex].SelectedEntities.Append(SelectionMessage.EntityTargets);
						}
						else {
							SelectionList[EntityIndex].SelectedEntities.Empty();
							SelectionList[EntityIndex].SelectedEntities.Append(SelectionMessage.EntityTargets);
						}
							
						// Clean out any dead entities
						TArray<int32> CurrentlySelectedDeleteIndexes;

						for (int32 SelectionIndex = 0; SelectionIndex < SelectionList[EntityIndex].SelectedEntities.Num(); ++SelectionIndex) {
							if (!SelectionList[EntityIndex].SelectedEntities[SelectionIndex].IsValid()) {
								CurrentlySelectedDeleteIndexes.Add(SelectionIndex);
							}
						}
						for (int32 SelectionDeleteIndexes = CurrentlySelectedDeleteIndexes.Num() - 1; SelectionDeleteIndexes > -1; --SelectionDeleteIndexes) {
							SelectionList[EntityIndex].SelectedEntities.RemoveAt(CurrentlySelectedDeleteIndexes[SelectionDeleteIndexes]);
						}


						// Message will be deleted by another processor
						SelectionSys->MarkModifySelectionMessageForDeletion(ModifySelectionMessagesIndex);
					}

				}
			}

			// See if any relevant SelectionEventMessages exist
			if (SelectionMessagesCount > 0) {
				auto LocalEntityHandle = Context.GetEntity(EntityIndex);

				// Continue looping so that Selection is most up to date
				for (int32 SelectionMessagesIndex = 0; SelectionMessagesIndex < SelectionMessagesCount; ++SelectionMessagesIndex) {
					auto SelectionMessage = SelectionSys->SelectionMessages[SelectionMessagesIndex];

					if (SelectionMessage.SelectionId == SelectionList[EntityIndex].SelectionId) {
						// Unload SelectionMessage

						SelectionSys->CurrentlySelectedEntities.Empty();
						SelectionSys->CurrentlySelectedEntities.Append(SelectionList[EntityIndex].SelectedEntities);

						SelectionSys->MarkSelectionMessageForDeletion(SelectionMessagesIndex);
					}

				}
			}
		}
	}));

	// If there are any SelectionMessages left, we probably need to create one
	auto ModifySelectionMessagesCount = SelectionSys->ModifySelectionMessages.Num();

	if (ModifySelectionMessagesCount > 0) {
		for (int32 ModifySelectionMessagesIndex = 0; ModifySelectionMessagesIndex < ModifySelectionMessagesCount; ++ModifySelectionMessagesIndex) {
			auto ModifySelectionMessage = SelectionSys->ModifySelectionMessages[ModifySelectionMessagesIndex];

			FMassEntityHandle SelectionEntity = CreateSelectionEntity();

			Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FSelectionFragment>>(SelectionEntity, CreateSelectionFragment(ModifySelectionMessage.EntityTargets, ModifySelectionMessage.SelectionId));
			
			// Queue up new SelectionMessage
			SelectionSys->AddSelectionWithIdMessage(ModifySelectionMessage.SelectionId);

			ModifySelectionMessage.MarkedForDeletion = true;
			SelectionSys->MarkModifySelectionMessageForDeletion(ModifySelectionMessagesIndex);	
		}
	}
	
	CleanupSelectionMessages();


	LastTickCalculated = SimTimeAuthoritySys->SimTimeAuthority.CurrentTick;
}

FMassEntityHandle USelectionPrc::CreateSelectionEntity()
{
	UMassEntitySubsystem* MassEntitySys = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	FMassEntityManager* EntityManager = &MassEntitySys->GetMutableEntityManager();

	FMassEntityHandle Entity = EntityManager->CreateEntity(SelectionEntityArchetype);

	EntityManager->CheckIfEntityIsValid(Entity);
	EntityManager->CheckIfEntityIsActive(Entity);


	return Entity;
}

FSelectionFragment USelectionPrc::CreateSelectionFragment(TArray<FMassEntityHandle> Entities, int32 SelectionId)
{
	return FSelectionFragment(Entities, SelectionId);
}

void USelectionPrc::CleanupSelectionMessages()
{
	auto SelectionMessagesCount = SelectionSys->SelectionMessages.Num();
	if (SelectionMessagesCount > 0) {

		TArray<int32> IndexesMarkedForDeletion;
		for (int32 SelectionMessagesIndex = 0; SelectionMessagesIndex < SelectionMessagesCount; ++SelectionMessagesIndex) {
			auto SelectionEventMessage = SelectionSys->SelectionMessages[SelectionMessagesIndex];

			if (SelectionEventMessage.MarkedForDeletion) {
				IndexesMarkedForDeletion.Add(SelectionMessagesIndex);
			}
		}

		// Cleanup messages
		for (int32 MarkedIndex = (IndexesMarkedForDeletion.Num() - 1); MarkedIndex > -1; --MarkedIndex) {
			SelectionSys->RemoveSelectionMessage(MarkedIndex);
		}
		// Resize messages queue
		SelectionSys->SelectionMessages.Shrink();

		// Will this re-arrange the array? This would be bad
	}

	auto ModifySelectionMessagesCount = SelectionSys->ModifySelectionMessages.Num();
	if (ModifySelectionMessagesCount > 0) {

		TArray<int32> IndexesMarkedForDeletion;
		for (int32 ModifySelectionMessagesIndex = 0; ModifySelectionMessagesIndex < ModifySelectionMessagesCount; ++ModifySelectionMessagesIndex) {
			auto ModifySelectionEventMessage = SelectionSys->ModifySelectionMessages[ModifySelectionMessagesIndex];

			if (ModifySelectionEventMessage.MarkedForDeletion) {
				IndexesMarkedForDeletion.Add(ModifySelectionMessagesIndex);
			}
		}

		// Cleanup messages
		for (int32 MarkedIndex = (IndexesMarkedForDeletion.Num() - 1); MarkedIndex > -1; --MarkedIndex) {
			SelectionSys->RemoveModifySelectionMessage(MarkedIndex);
		}
		// Resize messages queue
		SelectionSys->ModifySelectionMessages.Shrink();

		// Will this re-arrange the array? This would be bad
	}

	TArray<int32> CurrentlySelectedDeleteIndexes;

	for (int32 SelectionIndex = 0; SelectionIndex < SelectionSys->CurrentlySelectedEntities.Num(); ++SelectionIndex) {
		if (!SelectionSys->CurrentlySelectedEntities[SelectionIndex].IsValid()) {
			CurrentlySelectedDeleteIndexes.Add(SelectionIndex);
		}
	}
	for (int32 SelectionDeleteIndexes = CurrentlySelectedDeleteIndexes.Num() - 1; SelectionDeleteIndexes > -1; --SelectionDeleteIndexes) {
		SelectionSys->CurrentlySelectedEntities.RemoveAt(CurrentlySelectedDeleteIndexes[SelectionDeleteIndexes]);
	}
}
