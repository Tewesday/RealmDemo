// Copyright Epic Games, Inc. All Rights Reserved.


#include "RealmDemoGameModeBase.h"

void ARealmDemoGameModeBase::StartPlay()
{
	Super::StartPlay();

	UEntityDescriptionSys* EntityDescriptionSys = GetWorld()->GetSubsystem<UEntityDescriptionSys>();

	uint32 TEST_ENTITY_COUNT = 1;
	for (uint32 i = 0; i < TEST_ENTITY_COUNT; ++i) {
		FEntityDescriptionMessage EntityDescriptionMessage = EntityDescriptionSys->CreateStandardEntityDescription(nullptr);

		EntityDescriptionSys->EntityDescriptionMessages.Add(EntityDescriptionMessage);
	}
	
}

void ARealmDemoGameModeBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	DefaultPawnClass = ARTSPlayerPawn::StaticClass();
	PlayerControllerClass = ARTSPlayerController::StaticClass();
	HUDClass = ARTSPlayerHUD::StaticClass();

}
