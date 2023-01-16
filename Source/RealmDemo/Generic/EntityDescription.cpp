

#include "EntityDescription.h"

FIEntityDescription::~FIEntityDescription()
{
    Tags.Empty();
    Fragments.Empty();
}

UScriptStruct* FIEntityDescription::GetTagFromDescription(uint32 TagIndex, FIEntityDescription EntityDescription)
{
    switch (EntityDescription.Tags[TagIndex]) {
    case (uint32)(EGenericTags::IsDead): {
        return EntityDescription.CreateTagIsDead();
    }
    case (uint32)(EGenericTags::NeedsCleanup): {
        return EntityDescription.CreateTagNeedsCleanup();
    }
    }
    return nullptr;
}

FConstStructView FIEntityDescription::GetFragmentFromDescription(uint32 FragmentIndex, FIEntityDescription EntityDescription)
{
    return FConstStructView();
}

UScriptStruct* FIEntityDescription::CreateTagIsDead()
{
    return FIsDeadTag::StaticStruct();
}

UScriptStruct* FIEntityDescription::CreateTagNeedsCleanup()
{
    return FNeedsCleanup::StaticStruct();
}

void FIEntityDescription::AddTagToDescription(EGenericTags TagEnum, FIEntityDescription& EntityDescription)
{
    EntityDescription.Tags.Add((uint32)TagEnum);
}

TArray<FConstStructView> FIEntityDescription::GetAllFragmentsFromDescription()
{
    TArray<FConstStructView> ConstructedFragments;

    const uint32 NumberOfFragments = this->Fragments.Num();

    for (uint32 FragmentIndex = 0; FragmentIndex < NumberOfFragments; FragmentIndex++) {
        ConstructedFragments.Add(GetFragmentFromDescription(FragmentIndex, *this));
    }

    return ConstructedFragments;
}

void FIEntityDescription::AddAllFragmentsFromDescriptionToEntity(FMassEntityHandle Entity, FMassExecutionContext& Context)
{

}
