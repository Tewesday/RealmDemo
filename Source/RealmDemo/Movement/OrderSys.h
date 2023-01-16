

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "../Movement/MovementFragments.h"
#include "../Representation/SelectionSys.h"

#include "OrderSys.generated.h"

UENUM()
enum class EOrderType {
	Idle,
	MoveTo,
	Attack,
	Orders
};

/**
 * 
 */
UCLASS()
class REALMDEMO_API UOrderSys : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	TArray<FOrderEventMessage> OrderEventMessages;

	void InstructSelection(EOrderType OrderTypeTarget, FVector2D Location);
	void InstructSelectionWithTarget(EOrderType OrderTypeTarget, FMassEntityHandle EntityTarget);

	void AddOrderMessage(FMassEntityHandle Entity, EOrderType OrderTypeTarget, FVector2D Location);
	void AddOrderWithTargetMessage(FMassEntityHandle Entity, EOrderType OrderTypeTarget, FMassEntityHandle EntityTarget);

	void RemoveOrderMessage(int Index);

	void MarkOrderMessageForDeletion(int32 TargetIndex);

	void CleanupOrderMessages();

protected:

private:


};

USTRUCT()
struct REALMDEMO_API FOrderEventMessage
{
	GENERATED_BODY()

	// Entity this order belongs to
	FMassEntityHandle EntityTarget;

	// Target Entity (for chase/attacks)
	FMassEntityHandle OrderEntityTarget;

	// For simply moving to a location
	FVector2D OrderTargetLocation;

	// What the resulting OrderType should be
	EOrderType OrderTypeTarget;

	bool MarkedForDeletion = false;
};