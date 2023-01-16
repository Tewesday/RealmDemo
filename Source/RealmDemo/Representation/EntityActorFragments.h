

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTypes.h"
#include "StructUtils.h"
#include "StructView.h"

#include "AssetFileSettings.h"

#include "EntityActorFragments.generated.h"

UENUM()
enum class EEntityActorFragments {
    Actor,
    Mesh,
    EntityActorFragments
};

USTRUCT()
struct REALMDEMO_API FAActorFragment : public FMassFragment {
    GENERATED_BODY()

    FAActorFragment() = default;

    explicit FAActorFragment(AActor* ActorTarget)
        : Actor(ActorTarget)
    {
    }

    AActor* Actor = nullptr;
};


USTRUCT()
struct REALMDEMO_API FAMeshFragment : public FMassFragment
{
	GENERATED_BODY()

	FAMeshFragment() = default;

	explicit FAMeshFragment(uint32 MeshTypeTarget)
		: MeshType(MeshTypeTarget)
	{

	}

    inline static const uint32 MESH_TYPE_DEFAULT = MeshTypes::BaseMesh;

	uint32 MeshType = MESH_TYPE_DEFAULT;

	bool MeshUpToDate = false;

    USceneComponent* ActorSceneComponent = nullptr;

    FVector ActorScale3D = FVector();

    // Move to HISMFragment
    std::tuple<uint32, uint32> HismIndex;
    // Move to HISMFragment
    bool UsesHism = true;
};