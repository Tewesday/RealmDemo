

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SimTimeAuthority.h"
#include "SimTimeAuthoritySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API USimTimeAuthoritySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


	

public :


	USimTimeAuthoritySubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	FSimTimeAuthority SimTimeAuthority;

};
