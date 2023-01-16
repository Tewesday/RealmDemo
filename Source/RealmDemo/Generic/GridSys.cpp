


#include "GridSys.h"

void UGridSys::Initialize(FSubsystemCollectionBase& Collection)
{
	HexGrid.GenerateGrid();
}

void UGridSys::Deinitialize()
{
}

void UGridSys::DrawGrid(const UWorld* WorldContext, bool DrawNeighborConnections)
{
	HexGrid.DrawGrid(WorldContext, DrawNeighborConnections);
}
