

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassCommonTypes.h"
#include "MassCommandBuffer.h"
#include "MassCommands.h"

#include "../Generic/SimTimeAuthoritySubsystem.h"
#include "../Movement/OrderSys.h"
#include "../Movement/MovementFragments.h"
#include "../Generic/GenericTags.h"

#include "OrderCleanupPrc.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API UOrderCleanupPrc : public UMassProcessor
{
	GENERATED_BODY()

public:

protected:
	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context) override;

private:

	FMassEntityQuery OrderEntityQuery;

	USimTimeAuthoritySubsystem* SimTimeAuthoritySys;
	uint32 LastTickCalculated = INFINITY;

	UOrderSys* OrderSys;
};
