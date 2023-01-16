

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassCommonTypes.h"
#include "MassProcessor.h"
#include "MassCommandBuffer.h"
#include "MassCommands.h"
#include "Kismet/KismetMathLibrary.h"
//#include "Physics/PhysicsInterfacePhysX.h"

#include "../Generic/SimTimeAuthoritySubsystem.h"
#include "EntityActorSys.h"
#include "EntityActorFragments.h"
#include "../Generic/EntityDescriptionSys.h"
#include "../Generic/EntityDescriptionMessage.h"
#include "../Generic/GenericTags.h"
#include "AssetSys.h"

#include "EntityRenderPrc.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API UEntityRenderPrc : public UMassProcessor
{
	GENERATED_BODY()
public:

protected:
	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery ActorSceneQuery;
	FMassEntityQuery ActorMeshQuery;

	USimTimeAuthoritySubsystem* SimTimeAuthoritySys;
	uint32 LastTickCalculated = INFINITY;

	UAssetSys* AssetSys;
};
