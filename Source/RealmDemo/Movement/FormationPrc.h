

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "InstancedStruct.h"
#include "MassCommonTypes.h"
#include "MassEntitySubsystem.h"
#include "MassEntityTypes.h"
#include "MassEntityView.h"
#include "MassProcessor.h"
#include "StructUtils.h"
#include "StructView.h"
#include "VectorTypes.h"

#include "../Movement/MovementFragments.h"
#include "../Generic/SimTimeAuthoritySubsystem.h"
#include "FormationSys.h"

#include "FormationPrc.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API UFormationPrc : public UMassProcessor
{
	GENERATED_BODY()
public:
    virtual void Initialize(UObject& Owner) override;
    virtual void ConfigureQueries() override;
    virtual void Execute(FMassEntityManager& MassEntityManager, FMassExecutionContext& Context) override;

    FMassEntityQuery FormationMemberEntityQuery;
    FMassEntityQuery FormationEntityQuery;

    USimTimeAuthoritySubsystem* SimTimeAuthoritySys;

private:
    uint32 LastTickCalculated = 21;

    const int32 SEEK_WEIGHT = 1;
    const int32 SEPARATION_WEIGHT = 2;
    const int32 AVOID_WEIGHT = 5;

    UFormationSys* FormationSys;

    FMassArchetypeHandle FormationEntityArchetype;

    void AddMembersToFormation(FMFormationFragment& Formation);

    void UpdateFormationPosition(FMFormationFragment& Formation);

    void ApplyPositionsToEntitiesInFormation(FMFormationFragment& Formation);
};
