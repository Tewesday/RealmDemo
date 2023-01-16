

#include "RTSPlayerPawn.h"
#include "RTSPlayerController.h"
#include "RTSInputComponent.h"

// Sets default values
ARTSPlayerPawn::ARTSPlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bEnableCameraLag = false;
	SpringArmComponent->TargetArmLength = 500.0f;
	SpringArmComponent->bDoCollisionTest = false;
	// Set the angle of the camera
	auto Forward = SpringArmComponent->GetForwardVector();
	//SpringArmComponent->AddLocalOffset(FVector(-50.f, 100.f, -400.f));
	auto Rotator = UKismetMathLibrary::Conv_VectorToRotator(Forward);
	Rotator.Pitch = -90.f;
	SpringArmComponent->SetRelativeRotation(Rotator);
}

// Called when the game starts or when spawned
void ARTSPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARTSPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARTSPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	URTSInputComponent* PlayerInput = (URTSInputComponent*)PlayerInputComponent;
	//URTSInputComponent* PlayerInput = Cast<URTSInputComponent>(PlayerInputComponent);
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(GetController());

	auto* InputFileSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URTSInputFileSys>();
	BeginBoxSelect = InputFileSubsystem->GetBeginBoxSelect();
	BoxSelect = InputFileSubsystem->GetBoxSelect();
	EndBoxSelect = InputFileSubsystem->GetEndBoxSelect();
	BoxSelectContext = InputFileSubsystem->GetBoxSelectContext();

	PlayerInput->BindAction(BeginBoxSelect, ETriggerEvent::Triggered, this, &ARTSPlayerPawn::CallBeginBoxSelect);
	PlayerInput->BindAction(BoxSelect, ETriggerEvent::Triggered, this, &ARTSPlayerPawn::CallBoxSelect);
	PlayerInput->BindAction(EndBoxSelect, ETriggerEvent::Triggered, this, &ARTSPlayerPawn::CallEndBoxSelect);


	MoveTo = InputFileSubsystem->GetMoveTo();
	MoveToContext = InputFileSubsystem->GetMoveToContext();

	PlayerInput->BindAction(MoveTo, ETriggerEvent::Triggered, this, &ARTSPlayerPawn::CallMoveTo);
}

void ARTSPlayerPawn::PawnClientRestart()
{
	Super::PawnClientRestart();

	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
	Subsystem->ClearAllMappings();

	// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
	Subsystem->AddMappingContext(BoxSelectContext, 1);
	Subsystem->AddMappingContext(MoveToContext, 1);
}

void ARTSPlayerPawn::CallBeginBoxSelect()
{
	ARTSPlayerHUD* PlayerHUD = Cast<ARTSPlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	PlayerHUD->BeginBoxSelectDelegateObject.Execute();
}

void ARTSPlayerPawn::CallBoxSelect()
{
	ARTSPlayerHUD* PlayerHUD = Cast<ARTSPlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	PlayerHUD->BoxSelectDelegateObject.Execute();
}

void ARTSPlayerPawn::CallEndBoxSelect()
{
	ARTSPlayerHUD* PlayerHUD = Cast<ARTSPlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	PlayerHUD->EndBoxSelectDelegateObject.Execute();
}

void ARTSPlayerPawn::CallMoveTo()
{
	ARTSPlayerHUD* PlayerHUD = Cast<ARTSPlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	PlayerHUD->OrderMoveToLocationDelegateObject.Execute();
}
