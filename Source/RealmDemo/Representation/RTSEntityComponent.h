

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "MassEntityTypes.h"

#include "RTSEntityComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REALMDEMO_API UEntityComponent : public UActorComponent {
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UEntityComponent();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    FMassEntityHandle GetEntityHandle();

    void SetEntityHandle(FMassEntityHandle NewEntityHandle);

    int32 GetEntityActorIndex();

    void SetEntityActorIndex(int32 NewIndex);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:
    // Move to private with getters and setters
    // The entity that I am presenting
    FMassEntityHandle EntityHandle;

    // The entry index in SpawnSys for ease of removal
    int32 EntityActorIndexInSpawnSys = -1;
};
