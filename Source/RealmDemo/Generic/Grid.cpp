


#include "Grid.h"

Grid::Grid()
{
}

Grid::Grid(int32 Length, int32 Width)
{
	GridLength = Length;
	GridWidth = Width;
}

Grid::Grid(int32 Length, int32 Width, float Scale)
{
	GridLength = Length;
	GridWidth = Width;
	GridScale = Scale;
}

Grid::~Grid()
{
}


void Grid::GenerateGrid()
{
	for (int r = 0; r <= GridWidth; ++r) {
		int r_offset = floor(r / 2.0);
		for (int q = 0 - r_offset; q <= GridLength - r_offset; ++q) {

			FVector GridPosition = FVector(q, r, -q - r);

			TileGridPositions.Add(GridPosition);

			// Iterate through all Neighbors
			for (uint32 HexDirections = 0; HexDirections < HexDirection::Directions; ++HexDirections) {
				FVector PossibleNeighbor = CalculateHexNeighbor(GridPosition, HexDirections);

				// Find and add Neighbor to my Neighbors if my Neighbor exists
				int32 Neighbor = TileGridPositions.Find(PossibleNeighbor);
				if (Neighbor != INDEX_NONE) {
					TArray<FVector>* Neighbors = TileNeighborMap.Find(GridPosition);

					// Add Neighbor to my list
					if (Neighbors) {
						Neighbors->Add(PossibleNeighbor);
					}
					else {
						TArray<FVector> NewNeighbor = { PossibleNeighbor };
						TileNeighborMap.Add(GridPosition, NewNeighbor);
					}

					// Add myself to Neighbor
					TArray<FVector>* NeighborNeighbors = TileNeighborMap.Find(PossibleNeighbor);
					if (NeighborNeighbors) {
						NeighborNeighbors->Add(GridPosition);
					}
					else {
						TArray<FVector> NewNeighborNeighbor = { GridPosition };
						TileNeighborMap.Add(PossibleNeighbor, NewNeighborNeighbor);
					}

				}
			}
			
		}
	}
}

void Grid::DrawGrid(const UWorld* WorldContext, bool DrawNeighborConnections)
{
	float DrawHeight = 100.f;

	for (auto& GridPos : TileGridPositions) {
		FColor GridColor = FColor::MakeRandomColor();

		FVector2D GridWorldPos = GetHexWorldPosition(GridPos);
		FBox Box = FBox(FVector(GridWorldPos.X - (GridScale / 2), GridWorldPos.Y - (GridScale / 2), DrawHeight), FVector(GridWorldPos.X + (GridScale / 2), GridWorldPos.Y + (GridScale / 2), DrawHeight));
		DrawDebugBox(WorldContext, Box.GetCenter(), Box.GetExtent(), GridColor, true);

		if (DrawNeighborConnections) {
			for (auto& Neighbor : TileNeighborMap[GridPos]) {
				FVector2D NeighborGridWorldPos = GetHexWorldPosition(Neighbor);
				DrawDebugLine(WorldContext, FVector(GridWorldPos, DrawHeight), FVector(NeighborGridWorldPos, DrawHeight), GridColor, true);
			}
		}
	}
}

void Grid::SetGridWorldPosition(FVector2D WorldPosition)
{
	GridCenterWorldPosition = WorldPosition;
}

int32 Grid::CalculateHexLength(FVector HexGridPosition)
{
	return (abs(HexGridPosition.X) + abs(HexGridPosition.Y) + abs(HexGridPosition.Z)) / 2;
}

int32 Grid::CalculateHexDistance(FVector HexGridPositionA, FVector HexGridPositionB)
{
	return CalculateHexLength(HexGridPositionA - HexGridPositionB);
}

FVector Grid::GetHexDirectionOffset(int HexDirection)
{
	if (HexDirection < HexDirection::Directions) {
		switch (HexDirection) {
		case HexDirection::Right: {
			return FVector(1, 0, -1);
		}
		case HexDirection::TopRight: {
			return FVector(1, -1, 0);
		}
		case HexDirection::TopLeft: {
			return FVector(0, -1, 1);
		}
		case HexDirection::Left: {
			return FVector(-1, 0, 1);
		}
		case HexDirection::BottomLeft: {
			return FVector(-1, 1, 0);
		}
		case HexDirection::BottomRight: {
			return FVector(0, 1, -1);
		}
		}
	}

	return FVector();
}

FVector Grid::CalculateHexNeighbor(FVector HexGridPosition, int HexDirection)
{
	return HexGridPosition + GetHexDirectionOffset(HexDirection);
}

bool Grid::AreWeNeighbors(FVector HexGridPosition, FVector HexGridPositionPossibleNeighbor)
{
	for (int HexDirection = 0; HexDirection < HexDirection::Directions; ++HexDirection) {
		if (CalculateHexNeighbor(HexGridPositionPossibleNeighbor, HexDirection) == HexGridPosition) {
			return true;
		}
	}
	return false;
}

FVector2D Grid::GetOffsetWorldPosition(FVector HexGridPosition)
{
	float offset = -1;
	auto val = int(abs(HexGridPosition.Y));
	float col = HexGridPosition.X + ((HexGridPosition.Y + offset * (val % 2)) / 2);
	float row = HexGridPosition.Y;
	return FVector2D(col, row);
}

FVector2D Grid::CalculateHexWorldOffset(bool odd, float scale, FVector2D HexWorldPosition) {
	float width = (scale * sqrt(3));
	float height = 2 * scale;

	float horizontalOffset = HexWorldPosition.X * width;
	if (odd) {
		horizontalOffset += (width / 2);
	}
	float verticalOffset = HexWorldPosition.Y * (height * (3.0f / 4.0f));

	return FVector2D(horizontalOffset, verticalOffset) + GridCenterWorldPosition;
}

FVector2D Grid::CalculateHexCorner(int corner, float scale, FVector2D HexWorldPosition) {
	float angleDegree = 60.f * corner - 0.5f;
	float angleRadius = PI / 180 * angleDegree;
	return FVector2D(HexWorldPosition.X + scale * cos(angleRadius), HexWorldPosition.Y + scale * sin(angleRadius));
}

FVector2D Grid::GetHexWorldPosition(FVector HexGridPosition)
{
	const int32 r = int32(HexGridPosition.Y);
	return CalculateHexWorldOffset((r % 2 != 0), GridScale, (GetOffsetWorldPosition(HexGridPosition)));
}