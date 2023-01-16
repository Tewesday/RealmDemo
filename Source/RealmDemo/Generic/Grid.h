

#pragma once

#include "CoreMinimal.h"

//#include "Grid.generated.h"

enum HexDirection {
	Right,
	TopRight,
	TopLeft,
	Left,
	BottomLeft,
	BottomRight,
	Directions
};

/**
 * 
 */
class REALMDEMO_API Grid
{
	//GENERATED_BODY()

public:

	TArray<FVector> TileGridPositions;

	// K: TilePosition, V: Array of Neighbor TilePositions
	TMap<FVector, TArray<FVector>> TileNeighborMap;

	Grid();

	Grid(int32 Length, int32 Width);

	Grid(int32 Length, int32 Width, float Scale);

	~Grid();

	int32 CalculateHexLength(FVector HexGridPosition);

	int32 CalculateHexDistance(FVector HexGridPositionA, FVector HexGridPositionB);

	FVector GetHexDirectionOffset(int HexDirection);

	FVector CalculateHexNeighbor(FVector HexGridPosition, int HexDirection);

	bool AreWeNeighbors(FVector HexGridPositionA, FVector HexGridPositionB);

	FVector2D GetOffsetWorldPosition(FVector HexGridPosition);

	FVector2D CalculateHexWorldOffset(bool odd, float scale, FVector2D HexWorldPosition);

	FVector2D CalculateHexCorner(int corner, float scale, FVector2D HexWorldPosition);

	FVector2D GetHexWorldPosition(FVector HexGridPosition);


	//bool IsPositionCloserToNeighbors(FVector2D TargetPosition, FRTSTileFragment HexTile);

	void GenerateGrid();

	void DrawGrid(const UWorld* WorldContext, bool DrawNeighborConnections = false);

	void SetGridWorldPosition(FVector2D WorldPosition);

protected:


private:

	int32 GridLength = 8;
	int32 GridWidth = 8;

	int32 Layer = 0;

	float GridScale = 1000.f;

	FVector2D GridCenterWorldPosition = FVector2D::ZeroVector;
};
