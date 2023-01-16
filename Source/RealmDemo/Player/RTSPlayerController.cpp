


#include "RTSPlayerController.h"


void ARTSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//FInputModeGameAndUI inputMode;
	//inputMode.SetHideCursorDuringCapture(false);
	//SetInputMode(inputMode);
}

void ARTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI inputMode;
	inputMode.SetHideCursorDuringCapture(false);
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

	SetInputMode(inputMode);
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	//GetGameInstance()->GetSubsystem<URTSUISubsystem>()->SetupPlayerHUDUI();
	//Util::PrintDebugMessageToScreen(FString("RTSPlayerController BeginPlay"));
}

void ARTSPlayerController::Tick(float DeltaSeconds)
{
	// Might need a performance improvement
	IsMouseAtScreenEdge();
}

bool ARTSPlayerController::IsMouseAtScreenEdge()
{
	float MousePosX;
	float MousePosY;
	GetMousePosition(MousePosX, MousePosY);

	int32 ViewPortX;
	int32 ViewPortY;
	GetViewportSize(ViewPortX, ViewPortY);


	auto PlayerPawn = Cast<ARTSPlayerPawn>(GetPawn());

	bool Result = false;
	if (MousePosX >= (ViewPortX - EdgeLeniency)) {
		MovePlayerViewEast(PlayerPawn);
		Result = true;
	}
	if (MousePosY >= (ViewPortY - EdgeLeniency)) {
		MovePlayerViewSouth(PlayerPawn);
		Result = true;
	}

	if (MousePosX <= (0 + EdgeLeniency)) {
		MovePlayerViewWest(PlayerPawn);
		Result = true;
	}
	if (MousePosY <= (0 + EdgeLeniency)) {
		MovePlayerViewNorth(PlayerPawn);
		Result = true;
	}


	return Result;
}

void ARTSPlayerController::MovePlayerViewNorth(ARTSPlayerPawn* PlayerPawn)
{
	PlayerPawn->AddActorLocalOffset(FVector((EdgePanningSpeed), 0.f, 0.f));
}

void ARTSPlayerController::MovePlayerViewEast(ARTSPlayerPawn* PlayerPawn)
{
	PlayerPawn->AddActorLocalOffset(FVector(0.f, (EdgePanningSpeed), 0.f));
}

void ARTSPlayerController::MovePlayerViewSouth(ARTSPlayerPawn* PlayerPawn)
{
	PlayerPawn->AddActorLocalOffset(FVector(-(EdgePanningSpeed), 0.f, 0.f));
}

void ARTSPlayerController::MovePlayerViewWest(ARTSPlayerPawn* PlayerPawn)
{
	PlayerPawn->AddActorLocalOffset(FVector(0.f, -(EdgePanningSpeed), 0.f));
}
