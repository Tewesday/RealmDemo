

#include "RTSEntityActor.h"

// Sets default values
AEntityActor::AEntityActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    EntityComponent = CreateDefaultSubobject<UEntityComponent>(TEXT("EntityComponent"));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AEntityActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AEntityActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

UEntityComponent* AEntityActor::GetEntityComponent()
{
    return EntityComponent;
}

void AEntityActor::SetEntityComponent(UEntityComponent* NewEntityComponent)
{
    EntityComponent = NewEntityComponent;
}

UStaticMeshComponent* AEntityActor::GetStaticMeshComponent()
{
    return StaticMeshComponent;
}

void AEntityActor::SetStaticMeshComponent(UStaticMeshComponent* SMeshComponent)
{
    StaticMeshComponent = SMeshComponent;
}
