

#pragma once

#include "CoreMinimal.h"
#include "MassCommands.h"
#include "MassCommonTypes.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTypes.h"
#include "StructUtils.h"
#include "StructView.h"
#include "UObject/NoExportTypes.h"

#include "GenericTags.h"

#include "EntityDescription.generated.h"

// Needed for EntityDescriptionMessage. Add a new entry when creating a new EntityDescription type.
UENUM()
enum class EEntityDescriptions {
    Generic,
    Movement,
    EntityActor,
    Spawn,
    EntityDescriptionTypes
};

/**
 * A grouping of data to describe how an entity should be built
 * Extend to support a new module's tags and fragments
 */
USTRUCT()
struct REALMDEMO_API FIEntityDescription {
    GENERATED_BODY()

    virtual ~FIEntityDescription();

    // An array of the tags that this entity will be required to have
    TArray<uint32> Tags;

    // An array of the fragments that this entity will be required to have
    TArray<uint32> Fragments;

    static UScriptStruct* GetTagFromDescription(uint32 TagIndex, FIEntityDescription EntityDescription);

    static FConstStructView GetFragmentFromDescription(uint32 FragmentIndex, FIEntityDescription EntityDescription);

    static UScriptStruct* CreateTagIsDead();
    static UScriptStruct* CreateTagNeedsCleanup();

    static void AddTagToDescription(EGenericTags TagEnum, FIEntityDescription& EntityDescription);

    virtual TArray<FConstStructView> GetAllFragmentsFromDescription();

    //virtual FMassCommandAddFragmentInstances<FMassFragment> GetAllFragmentsFromDescriptionAsCommand();

    virtual void AddAllFragmentsFromDescriptionToEntity(FMassEntityHandle Entity, FMassExecutionContext& Context);
};

/**
 * A grouping of data to describe how an entity should be built
 */
USTRUCT()
struct REALMDEMO_API FEntityDescription : public FIEntityDescription {
    GENERATED_BODY()

    TTuple<> FragmentData;
};
