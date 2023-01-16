


#include "SimTimeAuthoritySubsystem.h"

USimTimeAuthoritySubsystem::USimTimeAuthoritySubsystem() 
{
	
}

void USimTimeAuthoritySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Log, TEXT("Initializing SimTimeAuthoritySubsystem"));
	SimTimeAuthority = FSimTimeAuthority();
}

void USimTimeAuthoritySubsystem::Deinitialize()
{
	//SimTimeAuthority = ;
}
