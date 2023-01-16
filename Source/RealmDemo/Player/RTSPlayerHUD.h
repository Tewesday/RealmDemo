

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MassEntityTypes.h"
#include "Delegates/Delegate.h"
#include "MassEntityView.h"
#include "EngineUtils.h"
#include "Engine/Canvas.h"

#include "../Representation/SelectionSys.h"
#include "../Movement/OrderSys.h"
#include "../Representation/EntityActorSys.h"
#include "../Generic/OctreeSys.h"

#include "RTSPlayerHUD.generated.h"

//UDELEGATE(BlueprintCallable)
DECLARE_DELEGATE(FBoxSelectDelegate);

//UDELEGATE(BlueprintCallable)
DECLARE_DELEGATE(FBeginBoxSelectDelegate);

//UDELEGATE(BlueprintCallable)
DECLARE_DELEGATE(FEndBoxSelectDelegate);

//UDELEGATE(BlueprintCallable)
DECLARE_DELEGATE(FOrderMoveToLocationDelegate);

/**
 * 
 */
UCLASS()
class REALMDEMO_API ARTSPlayerHUD : public AHUD
{
	GENERATED_BODY()
	

public:
	void PostInitializeComponents() override;
	void BeginDestroy() override;

	void DrawHUD() override;

	UFUNCTION(BlueprintCallable)
	void BeginBoxSelect();

	UFUNCTION(BlueprintCallable)
	void BoxSelect();

	UFUNCTION(BlueprintCallable)
	void EndBoxSelect();

	UPROPERTY(BlueprintReadOnly)
	bool ShouldDrawBoxSelect = false;

	TArray<FMassEntityHandle> GetActorsAsEntitiesInBox();
	TArray<FMassEntityHandle> GetEnemyActorsAsEntitiesInBox();

	TArray<FMassEntityHandle> GetEntitiesInBox();

	FVector2D MouseStartingPos;
	//FVector2D MouseCurrentPos;

	FVector2D HitResultAtMouseStartingPos;

	FBeginBoxSelectDelegate BeginBoxSelectDelegateObject;
	FBoxSelectDelegate BoxSelectDelegateObject;
	FEndBoxSelectDelegate EndBoxSelectDelegateObject;

	UFUNCTION(BlueprintCallable)
	void OrderMoveToLocation();

	FOrderMoveToLocationDelegate OrderMoveToLocationDelegateObject;

private:

	FVector2D GetLatestMousePosition();

	// Requires a physical object to hit
	FVector2D GetHitResultAtMousePosition();

	//FVector LastProjectedWorldLocation;
	UCanvas* CachedCanvas;

	const float ACTOR_SELECTION_SCALE_FACTOR = 5.f;

	FBox2D GetBox2DFromTwoCornerPoints(const FVector2D& FirstPoint, const FVector2D& SecondPoint);

	void PrivateInnerGetActorsInSelectionRectangle(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors);

	FVector PrivateProject(FVector Location, bool bClampToZeroPlane) const;
};
