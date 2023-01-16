

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RandomAuthoritySys.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API URandomAuthoritySys : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	FVector2D GetRandomPosition(double Min, double Max);

	FRandomStream Random;
};
