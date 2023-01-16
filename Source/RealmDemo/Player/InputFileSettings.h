

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "InputFileSettings.generated.h"

/**
 *
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Input File Settings"))
class REALMDEMO_API UInputFileSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//UInputFileSettings();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> BeginBoxSelect;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> BoxSelect;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> EndBoxSelect;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> BoxSelectContext;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> MoveTo;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> MoveToContext;

protected:

private:


};
