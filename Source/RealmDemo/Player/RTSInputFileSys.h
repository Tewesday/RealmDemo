

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "InputFileSettings.h"

#include "RTSInputFileSys.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API URTSInputFileSys : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	//UInputAction* BoxSelect;
	//UInputMappingContext* BoxSelectContext;

	UInputAction* GetBeginBoxSelect();
	UInputAction* GetBoxSelect();
	UInputAction* GetEndBoxSelect();
	UInputMappingContext* GetBoxSelectContext();

	UInputAction* GetMoveTo();
	UInputMappingContext* GetMoveToContext();

};
