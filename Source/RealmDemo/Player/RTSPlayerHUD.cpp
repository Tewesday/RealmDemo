


#include "RTSPlayerHUD.h"


void ARTSPlayerHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BeginBoxSelectDelegateObject.BindUObject(this, &ARTSPlayerHUD::BeginBoxSelect);
	BoxSelectDelegateObject.BindUObject(this, &ARTSPlayerHUD::BoxSelect);
	EndBoxSelectDelegateObject.BindUObject(this, &ARTSPlayerHUD::EndBoxSelect);

	OrderMoveToLocationDelegateObject.BindUObject(this, &ARTSPlayerHUD::OrderMoveToLocation);
}

void ARTSPlayerHUD::BeginDestroy()
{
	Super::BeginDestroy();

	BeginBoxSelectDelegateObject.Unbind();
	BoxSelectDelegateObject.Unbind();
	EndBoxSelectDelegateObject.Unbind();
	OrderMoveToLocationDelegateObject.Unbind();
}

void ARTSPlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	CachedCanvas = Canvas.Get();

	if (ShouldDrawBoxSelect) {

		FVector2D MouseCurrentPos = GetLatestMousePosition();
		DrawRect(FLinearColor(0.f, 255.f, 0.f, 0.25f), MouseStartingPos.X, MouseStartingPos.Y, MouseCurrentPos.X - MouseStartingPos.X, MouseCurrentPos.Y - MouseStartingPos.Y);

	}
}

void ARTSPlayerHUD::BeginBoxSelect()
{
	MouseStartingPos = GetLatestMousePosition();
	HitResultAtMouseStartingPos = GetHitResultAtMousePosition();

	ShouldDrawBoxSelect = true;
}

void ARTSPlayerHUD::BoxSelect()
{

	//Util::PrintDebugMessageToScreen(FString("HUD BoxSelect"));

	//MouseCurrentPos = GetLatestMousePosition();
	// Cannot draw outside of DrawHUD();
	//DrawRect(FLinearColor(0.f, 255.f, 0.f), MousePos.X, MousePos.Y, MouseStartingPos.X, MouseStartingPos.Y);
}

void ARTSPlayerHUD::EndBoxSelect()
{

	//TArray<FMassEntityHandle> Entities = GetActorsAsEntitiesInBox();
	TArray<FMassEntityHandle> Entities =  GetEntitiesInBox();
	//if (Entities.Num() > 0) {
	auto SelectionSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USelectionSys>();
	SelectionSubsystem->AddModifySelectionMessage(Entities);
	//}

	ShouldDrawBoxSelect = false;
}

TArray<FMassEntityHandle> ARTSPlayerHUD::GetActorsAsEntitiesInBox()
{
	FVector2D HitsResultAtMousePos = GetHitResultAtMousePosition();

	TArray<FMassEntityHandle> Entities;
	TArray<AActor*> Actors;

	auto MESubsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	auto EntityActorSys = GetWorld()->GetSubsystem<UEntityActorSys>();

	// Most Selection Boxes are square shaped so let's cheat and use spheres to detect. Long rectangles aren't quite as accurate, but it works for the game.
	FBox MouseSelectionBox = FBox(FVector(HitResultAtMouseStartingPos, -100.f), FVector(HitsResultAtMousePos, 100.f));
	FSphere MouseSelectionSphere = FSphere(MouseSelectionBox.GetCenter(), FVector2D::Distance(HitResultAtMouseStartingPos, HitsResultAtMousePos) / 2);

	Entities = EntityActorSys->GetEntitiesInSphere(MouseSelectionSphere);

	return Entities;
}

TArray<FMassEntityHandle> ARTSPlayerHUD::GetEnemyActorsAsEntitiesInBox()
{
	FVector2D HitsResultAtMousePos = GetHitResultAtMousePosition();

	TArray<FMassEntityHandle> Entities;
	TArray<AActor*> Actors;

	auto MESubsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	auto EntityActorSys = GetWorld()->GetSubsystem<UEntityActorSys>();

	// Most Selection Boxes are square shaped so let's cheat and use spheres to detect. Long rectangles aren't quite as accurate, but it works for the game.
	FBox MouseSelectionBox = FBox(FVector(HitResultAtMouseStartingPos, -100.f), FVector(HitsResultAtMousePos, 100.f));
	FSphere MouseSelectionSphere = FSphere(MouseSelectionBox.GetCenter(), FVector2D::Distance(HitResultAtMouseStartingPos, HitsResultAtMousePos) / 2);

	Entities = EntityActorSys->GetEntitiesInSphere(MouseSelectionSphere);

	return Entities;
}

TArray<FMassEntityHandle> ARTSPlayerHUD::GetEntitiesInBox()
{
	FVector2D HitsResultAtMousePos = GetHitResultAtMousePosition();

	TArray<FMassEntityHandle> Entities;
	TArray<AActor*> Actors;

	auto MESubsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	auto EntityActorSys = GetWorld()->GetSubsystem<UEntityActorSys>();
	auto OctreeSys = GetWorld()->GetGameInstance()->GetSubsystem<UOctreeSys>();

	// Most Selection Boxes are square shaped so let's cheat and use spheres to detect. Long rectangles aren't quite as accurate, but it works for the game.
	//FBox MouseSelectionBox = FBox(FVector(HitResultAtMouseStartingPos, -100.f), FVector(HitsResultAtMousePos, 100.f));
	//FSphere MouseSelectionSphere = FSphere(MouseSelectionBox.GetCenter(), FVector2D::Distance(HitResultAtMouseStartingPos, HitsResultAtMousePos) / 2);
	//FBox MouseSelectionBoxFromSphere = FBoxSphereBounds(MouseSelectionSphere).GetBox();

	//FBox HackedBox = FBox(FVector(-300, 300, -300), FVector(300, -300, 300));

	//DrawDebugBox(GetWorld(), MouseSelectionBoxFromSphere.GetCenter(), MouseSelectionBoxFromSphere.GetExtent(), FColor(100, 100, 100), false, 20.f);

	FBox2D MouseSelectionBox2D = GetBox2DFromTwoCornerPoints(HitResultAtMouseStartingPos, HitsResultAtMousePos);

	//Entities = EntityActorSys->GetEntitiesInSphere(MouseSelectionSphere);
	//Entities = OctreeSys->GetEntitiesInBox(HackedBox);
	Entities = OctreeSys->GetEntitiesInBox(FBoxCenterAndExtent(FVector(MouseSelectionBox2D.GetCenter(), 0.f), FVector(MouseSelectionBox2D.GetExtent(), 0.f)));

	//auto IDs = OctreeSys->GetIDsInSpace(HitResultAtMouseStartingPos, HitsResultAtMousePos);

	return Entities;
}

void ARTSPlayerHUD::OrderMoveToLocation()
{
	// Handle attack
	//TArray<FMassEntityHandle> Entities = GetEnemyActorsAsEntitiesInBox();

	//if (Entities.Num() > 0) {
	//	GetWorld()->GetSubsystem<UOrderSys>()->InstructSelectionWithTarget(EOrderType::Attack, Entities[0]);
	//}
	//else {
		// Handle move
		GetWorld()->GetSubsystem<UOrderSys>()->InstructSelection(EOrderType::MoveTo, GetHitResultAtMousePosition());
	//}

}

FVector2D ARTSPlayerHUD::GetLatestMousePosition()
{
	auto PlayerController = GetOwningPlayerController();
	float MouseX;
	float MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);
	return FVector2D(MouseX, MouseY);
}

// Requires a physical object to hit
FVector2D ARTSPlayerHUD::GetHitResultAtMousePosition() {
	auto PlayerController = GetOwningPlayerController();

	auto TraceChannel = ETraceTypeQuery();
	auto HitResult = FHitResult();

	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, HitResult);

	auto ImpactPoint = HitResult.ImpactPoint;
	return FVector2D(ImpactPoint);
}

FBox2D ARTSPlayerHUD::GetBox2DFromTwoCornerPoints(const FVector2D& FirstPoint, const FVector2D& SecondPoint)
{
	// How does this calculation work? https://www.desmos.com/calculator/hmnsqrmyp0

	// Calculate abs dimensions (avoid negative sizes)
	float Width = FMath::Abs(FirstPoint.X - SecondPoint.X);
	float Height = FMath::Abs(FirstPoint.Y - SecondPoint.Y);

	FVector2D CenterPoint = (FirstPoint + SecondPoint) / 2;

	FVector2D Extents = FVector2D(Width / 2, Height / 2);
	
	FVector2D Min = FVector2D(CenterPoint - Extents);

	FVector2D Max = FVector2D(CenterPoint + Extents);

	FBox2D BoxFromPoints = FBox2D(Min, Max);

	DrawDebugBox(GetWorld(), FVector(BoxFromPoints.GetCenter(), 0.f), FVector(BoxFromPoints.GetExtent(), 0.f), FColor(100, 100, 100), false, 20.f);

	return BoxFromPoints;
}
