

#pragma once

#include "CoreMinimal.h"
#include "MassCommonTypes.h"
#include "MassProcessor.h"
#include "MassCommandBuffer.h"

#include "../Generic/SimTimeAuthoritySubsystem.h"
#include "SpawnFragments.h"
#include "GenericTags.h"
#include "SpawnSys.h"
#include "EntityDescription.h"
#include "EntityDescriptionMessage.h"
#include "../Representation/AssetFileSettings.h"

#include "SpawnPrc.generated.h"

/**
 *
 */
UCLASS()
class REALMDEMO_API USpawnPrc : public UMassProcessor {
    GENERATED_BODY()

public:
    USpawnPrc();

protected:
    virtual void Initialize(UObject& Owner) override;
    virtual void ConfigureQueries() override;
    virtual void Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context) override;

private:
    FMassEntityQuery SpawnEntityQuery;

    USimTimeAuthoritySubsystem* SimTimeAuthoritySys;
    uint32 LastTickCalculated = INFINITY;
    
    USpawnSys* SpawnSys;

    TTuple<FMassEntityHandle, TArray<FConstStructView>> SpawnEntity(FEntityDescriptionMessage EntityDescriptionMessage);

    // Take all SpawnEventMessages in SpawnSys queue and give them to their Spawn Entities
    void DequeueSpawnEventMessages();

    // Take all SpawnEventMessages in SpawnSys queue and give them to their Spawn Entities within each Query Chunk
    void ChunkDequeueSpawnEventMessages(FMassEntityHandle Entity);

};
