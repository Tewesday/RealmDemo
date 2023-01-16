

#pragma once

#include "CoreMinimal.h"
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

#include "EntityActorPrc.generated.h"

/**
 *
 */
UCLASS()
class REALMDEMO_API UEntityActorPrc : public UMassProcessor {
	GENERATED_BODY()
public:
	UEntityActorPrc();

	// Thanks to vblanco for this fast transform setting trick
	// According to him the first two steps (comp to world and updatebounds) can be threaded but not the render dirty
	FORCEINLINE static void SetWorldTransformFastPath(USceneComponent* InComp, const FTransform& InTransform)
	{
		// directly set transform and update bounds 
		InComp->SetComponentToWorld(InTransform);
		InComp->UpdateBounds();

		// Evil static cast (yes this will explode instantly on non primitives) @TODO karl
		auto bodyinstance = static_cast<UPrimitiveComponent*>(InComp)->BodyInstance;
		FChaosEngineInterface::SetGlobalPose_AssumesLocked(bodyinstance.ActorHandle, InTransform);

		// dirty the render transform 
		InComp->MarkRenderTransformDirty();

		for (auto Component : InComp->GetAttachChildren())
		{
			// This * transforms from local space to world space!
			FTransform CompWorldTransform = Component->GetRelativeTransform() * InTransform;

			//These are to support non-relative transforms (could probably omit as this is rare?)

			if (UNLIKELY(InComp->IsUsingAbsoluteLocation()))
			{
				CompWorldTransform.CopyTranslation(InTransform);
			}

			if (UNLIKELY(InComp->IsUsingAbsoluteRotation()))
			{
				CompWorldTransform.CopyRotation(InTransform);
			}

			if (UNLIKELY(InComp->IsUsingAbsoluteScale()))
			{
				CompWorldTransform.CopyScale3D(InTransform);
			}

			// Recursive!
			SetWorldTransformFastPath(Component, CompWorldTransform);
		}
	};

protected:
	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery ActorEntityQuery;

	USimTimeAuthoritySubsystem* SimTimeAuthoritySys;
	uint32 LastTickCalculated = INFINITY;

	UEntityActorSys* EntityActorSys;

	UAssetSys* AssetSys;

	UEntityDescriptionSys* EntityDescriptionSys;
};
