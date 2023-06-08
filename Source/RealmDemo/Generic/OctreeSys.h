

#pragma once

#include "CoreMinimal.h"
#include "MassCommonTypes.h"
#include "MassCommandBuffer.h"
#include "MassCommands.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Spatial/SparseDynamicOctree3.h"

//#include "octree.h"
//#include "octree_container.h"
#include "../Representation/EntityActorSys.h"

#include "OctreeSys.generated.h"

/**
 * 
 */
UCLASS()
class REALMDEMO_API UOctreeSys : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	uint32 TreeIDs;
	TMap<uint32, FMassEntityHandle> TreeIDToEntityHandleMap;

	// TODO: Compare performance of core vs container
	//OrthoTree::QuadtreeBox QuadTree;
	//OrthoTree::QuadtreeBoxC QuadTree;

	//OrthoTree::OctreePointC PointTree;
	
	UE::Geometry::FSparseDynamicOctree3 QuadTree;

	//std::vector<OrthoTree::BoundingBox2D> BoundingBoxes;

	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	TArray<FMassEntityHandle> GetEntitiesInBox(const FBoxCenterAndExtent& QueryBox);
	//std::vector<OrthoTree::entity_id_type> GetIDsInSpace(FVector2D PointA, FVector2D PointB);

	//void VisualizeOctree();

protected:

private:

	int32 OctreeBoundary = 3000;

	UE::Geometry::FAxisAlignedBox3d QuadBoundingBox = UE::Geometry::FAxisAlignedBox3d(FBox());

	//OrthoTree::BoundingBox2D OctreeBoundingBox = OrthoTree::BoundingBox2D({ -OctreeBoundary, -OctreeBoundary }, { OctreeBoundary, OctreeBoundary });

	//OrthoTree::BoundingBox3D PointTreeBoundingBox = OrthoTree::BoundingBox3D({ -OctreeBoundary, -OctreeBoundary }, { OctreeBoundary, OctreeBoundary });
};
