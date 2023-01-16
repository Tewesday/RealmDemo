

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"

#include "../Generic/SimTimeAuthoritySubsystem.h"
#include "../Movement/MovementFragments.h"
#include "GridSys.h"

#include "GridPrc.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API UGridPrc : public UMassProcessor
{
	GENERATED_BODY()
public:
    virtual void Initialize(UObject& Owner) override;
    virtual void ConfigureQueries() override;
    virtual void Execute(FMassEntityManager& MassEntityManager, FMassExecutionContext& Context) override;

    FMassEntityQuery GridEntityQuery;

private:

    USimTimeAuthoritySubsystem* SimTimeAuthoritySys;

    uint32 LastTickCalculated = 21;

    UGridSys* GridSys;

};
