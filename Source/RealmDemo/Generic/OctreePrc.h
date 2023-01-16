

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassCommonTypes.h"
#include "MassCommandBuffer.h"
#include "MassCommands.h"
#include "Kismet/KismetMathLibrary.h"

#include "MOctree.h"
#include "OctreeSys.h"
#include "../Generic/SimTimeAuthoritySubsystem.h"
#include "../Movement/MovementFragments.h"
#include "../Representation/EntityActorSys.h"

#include "OctreePrc.generated.h"

DECLARE_STATS_GROUP(TEXT("InsertOctree"), STATGROUP_INSERTOCTREE, STATCAT_Advanced);
DECLARE_STATS_GROUP(TEXT("UpdateOctree"), STATGROUP_UPDATEOCTREE, STATCAT_Advanced);

/**
 * 
 */
UCLASS()
class REALMDEMO_API UOctreePrc : public UMassProcessor
{
	GENERATED_BODY()
	
public:

	FCriticalSection criticalSection;

protected:
	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery OctreeQuery;

	USimTimeAuthoritySubsystem* SimTimeAuthoritySys;
	uint32 LastTickCalculated = INFINITY;

	UOctreeSys* OctreeSys;

	UEntityActorSys* EntityActorSys;
};
