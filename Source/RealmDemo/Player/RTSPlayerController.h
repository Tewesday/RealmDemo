

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "RTSPlayerPawn.h"
#include "RTSUISys.h"

#include "RTSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void PostInitializeComponents() override;
	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	bool IsMouseAtScreenEdge();

	void MovePlayerViewNorth(ARTSPlayerPawn* PlayerPawn);
	void MovePlayerViewEast(ARTSPlayerPawn* PlayerPawn);
	void MovePlayerViewSouth(ARTSPlayerPawn* PlayerPawn);
	void MovePlayerViewWest(ARTSPlayerPawn* PlayerPawn);

private:

	const float EdgeLeniency = 15.f;

	// Should be adjustable by player
	float EdgePanningSpeed = 15.f;
};
