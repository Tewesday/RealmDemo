

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassCommonTypes.h"
#include "MassCommandBuffer.h"
#include "MassCommands.h"
#include "MassEntitySubsystem.h"
#include "MassEntityTypes.h"
#include "MassEntityView.h"
#include "StructUtils.h"
#include "StructView.h"
#include "InstancedStruct.h"
#include "Subsystems/WorldSubsystem.h"

#include "SelectionSys.h"
#include "../Generic/SimTimeAuthoritySubsystem.h"

#include "SelectionPrc.generated.h"

/**
 *
 */
UCLASS()
class REALMDEMO_API USelectionPrc : public UMassProcessor
{
	GENERATED_BODY()

public:
	//USelectionPrc();

protected:
	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery SelectionEntityQuery;
	USelectionSys* SelectionSys;

	USimTimeAuthoritySubsystem* SimTimeAuthoritySys;
	uint32 LastTickCalculated = INFINITY;

	FMassArchetypeHandle SelectionEntityArchetype;

	FMassEntityHandle CreateSelectionEntity();

	FSelectionFragment CreateSelectionFragment(TArray<FMassEntityHandle> Entities, int32 SelectionId);

	void CleanupSelectionMessages();
};
