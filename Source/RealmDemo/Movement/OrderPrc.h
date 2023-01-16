

#pragma once

#include "CoreMinimal.h"
#include "MassCommonTypes.h"
#include "MassProcessor.h"
#include "MassCommandBuffer.h"
#include "MassCommands.h"
#include "Kismet/KismetMathLibrary.h"
//#include "Physics/PhysicsInterfacePhysX.h"

#include "../Generic/SimTimeAuthoritySubsystem.h"
#include "../Movement/OrderSys.h"
#include "../Movement/MovementFragments.h"
#include "../Generic/GenericTags.h"

#include "OrderPrc.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API UOrderPrc : public UMassProcessor
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

	FOrderEventMessage GetLatestOrderForEntity(FMassEntityHandle EntityHandle);

	EMoveOrderType ConvertOrderTypeToMoveOrderType(EOrderType OrderType);
};
