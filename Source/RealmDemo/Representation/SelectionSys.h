

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MassEntityTypes.h"



#include "SelectionSys.generated.h"

/**
 *
 */
UCLASS()
class REALMDEMO_API USelectionSys : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	TArray<FModifySelectionEventMessage> ModifySelectionMessages;
	TArray<FSelectionEventMessage> SelectionMessages;
	TArray<FMassEntityHandle> CurrentlySelectedEntities;

	void AddModifySelectionMessage(TArray<FMassEntityHandle> Entities);
	void AddModifySelectionWithIdMessage(TArray<FMassEntityHandle> Entities, int32 SelectionId);

	void RemoveModifySelectionMessage(int Index);

	// Get selection -1
	void AddSelectionMessage();

	// Get a specific selection
	void AddSelectionWithIdMessage(int32 SelectionId);

	void RemoveSelectionMessage(int Index);

	void MarkModifySelectionMessageForDeletion(int32 TargetIndex);

	void MarkSelectionMessageForDeletion(int32 TargetIndex);

protected:

private:


};


/**
 *
 */
USTRUCT()
struct REALMDEMO_API FModifySelectionEventMessage
{
	GENERATED_BODY()

	TArray<FMassEntityHandle> EntityTargets;

	// Selection "key"
	int32 SelectionId = -1;

	bool ShouldAppend = false;

	bool MarkedForDeletion = false;
};

/**
 *
 */
USTRUCT()
struct REALMDEMO_API FSelectionEventMessage
{
	GENERATED_BODY()

	// Selection "key"
	int32 SelectionId = -1;

	bool MarkedForDeletion = false;
};

/*
* Not intended to "exist" as part of a traditional entity. Only used for current selection and control groups by the player.
*/
USTRUCT()
struct REALMDEMO_API FSelectionFragment : public FMassFragment
{
	GENERATED_BODY()

	FSelectionFragment(TArray<FMassEntityHandle> Entities, int32 NewSelectionId) {
		SelectedEntities = Entities;
		SelectionId = NewSelectionId;
	}

	FSelectionFragment() = default;

	// The entities in this selection
	TArray<FMassEntityHandle> SelectedEntities;

	// The selection "key"
	int32 SelectionId = -1;

	//const int32 CURRENT_SELECTION = -1;
};