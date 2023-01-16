

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "RTSUISys.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API URTSUISys : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void SetupPlayerHUDUI();

	//URTSUserWidget* PlayerHUDWidget;

protected:



private:


};
