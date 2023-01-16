

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"



#include "AssetFileSettings.generated.h"

enum MeshTypes {
	BaseMesh, // A standard placeholder mesh
	Types
};

/**
 *
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Asset File Settings"))
class REALMDEMO_API UAssetFileSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//UAssetFileSettings();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Assets")
	TSoftObjectPtr<UStaticMesh> BaseMesh;

protected:

private:


};
