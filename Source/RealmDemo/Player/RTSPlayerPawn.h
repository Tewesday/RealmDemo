

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h" 
#include "EnhancedInputSubsystems.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "RTSPlayerHUD.h"
#include "RTSPlayerPawn.h"
#include "RTSInputFileSys.h"

#include "RTSInputComponent.h"

#include "RTSPlayerPawn.generated.h"


UCLASS()
class REALMDEMO_API ARTSPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void PawnClientRestart() override;

	UInputAction* BeginBoxSelect;
	UInputAction* BoxSelect;
	UInputAction* EndBoxSelect;
	UInputMappingContext* BoxSelectContext;


	void CallBeginBoxSelect();
	void CallBoxSelect();
	void CallEndBoxSelect();

	UInputAction* MoveTo;
	UInputMappingContext* MoveToContext;

	void CallMoveTo();

	UStaticMeshComponent* StaticMeshComponent;

	USpringArmComponent* SpringArmComponent;

	UCameraComponent* CameraComponent;
};
