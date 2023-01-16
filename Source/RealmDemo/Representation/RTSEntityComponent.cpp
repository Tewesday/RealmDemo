

#include "RTSEntityComponent.h"

// Sets default values for this component's properties
UEntityComponent::UEntityComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UEntityComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

// Called every frame
void UEntityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

FMassEntityHandle UEntityComponent::GetEntityHandle()
{
    return EntityHandle;
}

void UEntityComponent::SetEntityHandle(FMassEntityHandle NewEntityHandle)
{
    EntityHandle = NewEntityHandle;
}

int32 UEntityComponent::GetEntityActorIndex()
{
    return EntityActorIndexInSpawnSys;
}

void UEntityComponent::SetEntityActorIndex(int32 NewIndex)
{
    EntityActorIndexInSpawnSys = NewIndex;
}
