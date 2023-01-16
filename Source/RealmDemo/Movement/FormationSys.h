

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MassProcessor.h"
#include "MassCommonTypes.h"
#include "MassEntityView.h"
#include "VectorTypes.h"

#include "../Movement/MovementFragments.h"
#include "../Generic/SimTimeAuthoritySubsystem.h"

#include "FormationSys.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API UFormationSys : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	TArray<FMassEntityHandle> FormationHandles;

	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

protected:

private:
};
