


#include "OctreeSys.h"

void UOctreeSys::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	//QuadTree = OrthoTree::QuadtreeBoxC();
	auto boxes = std::vector
	{
	  OrthoTree::BoundingBox2D{ { 1000000000.0, 1000000000.0 }, { 10000000000.0, 1000000000.0 } },
	};

	QuadTree = OrthoTree::QuadtreeBoxC(boxes, 3
		, OctreeBoundingBox // user-provided bounding box for all
		, 8            // max element in a node 
		, false        // parallel calculation flag
	);

	PointTree.Init(PointTreeBoundingBox, 3);

	//QuadTree.Init(OctreeBoundingBox, 2);
	//QuadTree.nSplitStrategyAdditionalDepth
}

void UOctreeSys::Deinitialize()
{
}

TArray<FMassEntityHandle> UOctreeSys::GetEntitiesInBox(const FBoxCenterAndExtent& QueryBox)
{
	TArray<FMassEntityHandle> EntitesInBox;

	FBox Box = QueryBox.GetBox();
	auto AABB = FBox::BuildAABB(QueryBox.Center, QueryBox.Extent);

	auto search_box = OrthoTree::BoundingBox2D{ { AABB.Min.X, AABB.Min.Y  }, { AABB.Max.X, AABB.Max.Y } };
	std::vector<OrthoTree::entity_id_type> IDs = QuadTree.RangeSearch<false /*overlap allowed*/>(search_box);

	UEntityActorSys* EntityActorSys = GetWorld()->GetSubsystem<UEntityActorSys>();
	TArray<FMassEntityHandle> EntityHandles = EntityActorSys->GetEntitiesHandles();

	// Remove references from components
	for (int32 index = 0; index < EntityHandles.Num(); ++index) {
		FMassEntityView EntityView = EntityActorSys->GetEntityView(EntityHandles[index]);
		auto* OctreeFragment = EntityView.GetFragmentDataPtr<FMOctreeFragment>();
		for (int32 IDsIndex = 0; IDsIndex < IDs.size(); ++IDsIndex) {
			if (OctreeFragment->OctreeID == IDs[IDsIndex]) {
				EntitesInBox.Add(EntityHandles[index]);
			}
		}
		
	}

	return EntitesInBox;
}

std::vector<OrthoTree::entity_id_type> UOctreeSys::GetIDsInSpace(FVector2D PointA, FVector2D PointB)
{
	auto search_box = OrthoTree::BoundingBox2D{ { PointA.X, PointA.Y  }, { PointB.X, PointB.Y } };
	std::vector<OrthoTree::entity_id_type> IDs = QuadTree.RangeSearch<false /*overlap allowed*/>(search_box);
	return IDs;
}

void UOctreeSys::VisualizeOctree()
{
	BoundingBoxes = QuadTree.GetData();
	for (auto box : BoundingBoxes) {
		FBox Box = FBox(FVector(box.Min[0], box.Min[1], 0), FVector(box.Max[0], box.Max[1], 0));
		DrawDebugBox(GetWorld(), Box.GetCenter(), Box.GetExtent(), FColor::MakeRandomColor(), false, 0.1f);
	}
}