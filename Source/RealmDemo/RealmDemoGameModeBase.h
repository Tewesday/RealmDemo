// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "./Generic/EntityDescriptionSys.h"
#include "./Representation/AssetFileSettings.h"
#include "./Player/RTSPlayerHUD.h"
#include "./Player/RTSPlayerController.h"
#include "./Player/RTSPlayerPawn.h"

#include "RealmDemoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API ARealmDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void StartPlay() override;

	void PreInitializeComponents() override;

};
