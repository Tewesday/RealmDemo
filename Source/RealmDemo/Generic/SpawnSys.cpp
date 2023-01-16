

#include "SpawnSys.h"

void USpawnSys::Initialize(FSubsystemCollectionBase& Collection)
{
}

void USpawnSys::Deinitialize()
{
}

void USpawnSys::SendEntityDescriptionMessageToQueue(FEntityDescriptionMessage EntityDescriptionMessage, FMassEntityHandle EntityTarget)
{
    FSpawnEventMessage SpawnEventMessage;

    SpawnEventMessage.EntityDescription = EntityDescriptionMessage;
    SpawnEventMessage.SpawnEntityTarget = EntityTarget;
    SpawnEventMessage.TimeRequiredToSpawn = DEFAULT_TIME_REQUIRED;

    SpawnEventMessages.Add(SpawnEventMessage);
}
