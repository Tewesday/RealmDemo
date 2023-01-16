


#include "RTSUISys.h"


void URTSUISys::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//const UUIFileSettings* UIFileSettings = GetDefault<UUIFileSettings>();


}

void URTSUISys::SetupPlayerHUDUI()
{
	//const UUIFileSettings* UIFileSettings = GetDefault<UUIFileSettings>();
	//UIFileSettings->HUDBase.LoadSynchronous();
	//FString HUDBaseString = (UIFileSettings->HUDBase.GetAssetName() + UIFileSettings->HUDBase.GetLongPackageName());
	//UClass* HUDBaseObject = UIFileSettings->HUDBase.Get();
	//ARTSPlayerHUD* PlayerHUD = Cast<ARTSPlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	//UUserWidget* HUDBaseWidget = CreateWidget(GetWorld(), HUDBaseObject, FName("WG_HUDBase"));
	//HUDBaseWidget->AddToViewport();

	//PlayerHUDWidget = Cast<URTSUserWidget>(HUDBaseWidget);
	//if (PlayerHUDWidget != nullptr) {
		//PlayerHUDWidget->FindAllWidgets();
	//}

	//URTSUserWidget* HUDBase = ->GetHUDBase();
	//HUDBase->CreateWidgetInstance
	//HUDBase->AddToViewport();

	//ARTSPlayerHUD* PlayerHUD = Cast<ARTSPlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

}
