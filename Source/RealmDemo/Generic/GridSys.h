

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Grid.h"

#include "GridSys.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API UGridSys : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	void DrawGrid(const UWorld* WorldContext, bool DrawNeighborConnections = false);

protected:

private:

	Grid HexGrid;

};
