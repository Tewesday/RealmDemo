

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTSEntityComponent.h"

#include "RTSEntityActor.generated.h"

UCLASS()
class REALMDEMO_API AEntityActor : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AEntityActor();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UEntityComponent* GetEntityComponent();

    void SetEntityComponent(UEntityComponent* NewEntityComponent);

    UStaticMeshComponent* GetStaticMeshComponent();

    void SetStaticMeshComponent(UStaticMeshComponent* SMeshComponent);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // Reference to MassEntity
    UPROPERTY()
    UEntityComponent* EntityComponent;

    // Visual mesh
    UPROPERTY()
    UStaticMeshComponent* StaticMeshComponent;

};
