

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MassProcessor.h"
#include "Subsystems/WorldSubsystem.h"
#include "MassEntityTypes.h"
#include "MassEntitySubsystem.h"
#include "MassCommonTypes.h"
#include "MassEntityView.h"
#include "StructUtils.h"
#include "StructView.h"
#include "InstancedStruct.h"

#include "../Representation/RTSEntityActor.h"
#include "../Representation/RTSEntityComponent.h"
#include "SpawnFragments.h"
#include "GenericTags.h"
#include "EntityDescriptionMessage.h"
#include "../Representation/AssetFileSettings.h"

#include "SpawnSys.generated.h"

/**
 *
 */
UCLASS()
class REALMDEMO_API USpawnSys : public UWorldSubsystem {
    GENERATED_BODY()

public:

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;

    // Spawn Event Messages as a queue
    TArray<FSpawnEventMessage> SpawnEventMessages;

    /*
     * Given an already made EntityDescription and the EntityTarget in charge of spawning it, add a SpawnEventMessage to the Queue
     */
    void SendEntityDescriptionMessageToQueue(FEntityDescriptionMessage EntityDescriptionMessage, FMassEntityHandle EntityTarget);

    // void MarkSpawnEventMessageForDeletion(int32 TargetIndex);

    const double DEFAULT_TIME_REQUIRED = 5.0;

private:

};
