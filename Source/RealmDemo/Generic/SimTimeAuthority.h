

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Stats/Stats2.h"

/**
 * 
 */
class REALMDEMO_API FSimTimeAuthority : public FTickableGameObject
{
public:
	FSimTimeAuthority();
	~FSimTimeAuthority();

	bool GameIsPaused = false;

	// Tick rate (ms) ms per tick
	double TickRate = 0.0167;

	// Time passed so far
	double CurrentTickAmount = 0;

	// Ticks in a turn. 20 50ms ticks. 1 turn = 1 second.
	uint32 TurnTicks = 20;

	// Current tick of turn (if current tick of turn is == ticks in a turn, advance turn)
	uint32 CurrentTick = 0;

	// Current turn
	uint64 CurrentTurn = 0;

	// Communication turn (how many turns in the future orders are executed)
	uint32 CommunicationTurn = 2;

	// Expected foreign input delay (if this spikes, react to increased latency)
	double ForeignInputDelay = 0.0;

	// Avg foreign input delay
	double AvgForeignInputDelay = 0.0;

	// Buffer of foreign input delays, Init to something like [100,20] [value, cols]
	TArray<double> ForeignInputDelays;



	// FTickableGameObject implementation begin

	virtual void Tick(float DeltaTime) override;

	virtual ETickableTickType GetTickableTickType() const override 
	{
		return ETickableTickType::Always;
	}

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FSimTimeAuthority, STATGROUP_Tickables);
	}

	virtual bool IsTickableWhenPaused() const
	{
		return true;
	}

	virtual bool IsTickableInEditor() const
	{
		return false;
	}

	// FTickableGameObject implementation end

private:

	uint32 LastFrameTicked = INDEX_NONE;
};
