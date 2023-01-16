


#include "RandomAuthoritySys.h"

void URandomAuthoritySys::Initialize(FSubsystemCollectionBase& Collection)
{
	Random = FRandomStream();
	Random.GenerateNewSeed();
}

void URandomAuthoritySys::Deinitialize()
{
}

FVector2D URandomAuthoritySys::GetRandomPosition(double Min, double Max)
{
	double RandomDoubleX = Random.FRandRange(Min, Max);
	double RandomDoubleY = Random.FRandRange(Min, Max);

	return FVector2D(RandomDoubleX, RandomDoubleY);
}
