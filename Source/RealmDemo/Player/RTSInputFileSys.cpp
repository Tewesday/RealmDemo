


#include "RTSInputFileSys.h"


void URTSInputFileSys::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UInputFileSettings* InputFileSettings = GetDefault<UInputFileSettings>();

	auto BeginBoxSelect = InputFileSettings->BeginBoxSelect.LoadSynchronous();
	auto BoxSelect = InputFileSettings->BoxSelect.LoadSynchronous();
	auto EndBoxSelect = InputFileSettings->EndBoxSelect.LoadSynchronous();
	auto BoxSelectContext = InputFileSettings->BoxSelectContext.LoadSynchronous();

	auto MoveTo = InputFileSettings->MoveTo.LoadSynchronous();
	auto MoveToContext = InputFileSettings->MoveToContext.LoadSynchronous();
}

UInputAction* URTSInputFileSys::GetBeginBoxSelect()
{
	const UInputFileSettings* InputFileSettings = GetDefault<UInputFileSettings>();

	auto BeginBoxSelect = InputFileSettings->BeginBoxSelect.LoadSynchronous();

	return InputFileSettings->BeginBoxSelect.Get();
}

UInputAction* URTSInputFileSys::GetBoxSelect()
{
	const UInputFileSettings* InputFileSettings = GetDefault<UInputFileSettings>();

	auto BoxSelect = InputFileSettings->BoxSelect.LoadSynchronous();

	return InputFileSettings->BoxSelect.Get();
}

UInputAction* URTSInputFileSys::GetEndBoxSelect()
{
	const UInputFileSettings* InputFileSettings = GetDefault<UInputFileSettings>();

	auto EndBoxSelect = InputFileSettings->EndBoxSelect.LoadSynchronous();

	return InputFileSettings->EndBoxSelect.Get();
}

UInputMappingContext* URTSInputFileSys::GetBoxSelectContext()
{
	const UInputFileSettings* InputFileSettings = GetDefault<UInputFileSettings>();

	auto BoxSelectContext = InputFileSettings->BoxSelectContext.LoadSynchronous();

	return InputFileSettings->BoxSelectContext.Get();
}

UInputAction* URTSInputFileSys::GetMoveTo()
{
	const UInputFileSettings* InputFileSettings = GetDefault<UInputFileSettings>();

	auto MoveTo = InputFileSettings->MoveTo.LoadSynchronous();

	return InputFileSettings->MoveTo.Get();
}

UInputMappingContext* URTSInputFileSys::GetMoveToContext()
{
	const UInputFileSettings* InputFileSettings = GetDefault<UInputFileSettings>();

	auto MoveToContext = InputFileSettings->MoveToContext.LoadSynchronous();

	return InputFileSettings->MoveToContext.Get();
}