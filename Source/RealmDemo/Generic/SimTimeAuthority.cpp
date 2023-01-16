


#include "SimTimeAuthority.h"

FSimTimeAuthority::FSimTimeAuthority()
{
}

FSimTimeAuthority::~FSimTimeAuthority()
{
}

void FSimTimeAuthority::Tick(float DeltaTime)
{
	if (LastFrameTicked == GFrameCounter ||
		GameIsPaused) {
		return;
	}

	// Update time
	CurrentTickAmount += DeltaTime;

	// If time passed is equal to TickRate, advance one game tick
	// No matter how slow the engine's tick rate becomes, any time greater than TickRate will advance one tick for consistency
	if (CurrentTickAmount >= TickRate) {
		CurrentTick += 1;
		CurrentTickAmount = 0;

		if (CurrentTick >= TurnTicks) {
			CurrentTurn += 1;
			CurrentTick = 0;
		}
	}


	LastFrameTicked = GFrameCounter;
}