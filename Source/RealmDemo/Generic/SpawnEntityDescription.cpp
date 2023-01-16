

#include "SpawnEntityDescription.h"

FConstStructView FSpawnEntityDescription::GetSpawnFragmentFromDescription(uint32 FragmentIndex, FSpawnEntityDescription EntityDescription)
{
    switch (EntityDescription.SpawnFragments[FragmentIndex]) {
    case (uint32)(ESpawnFragments::Spawn): {
        return CreateFragmentSpawn(EntityDescription);
    }
    }
    return FConstStructView();
}

TArray<FConstStructView> FSpawnEntityDescription::GetAllFragmentsFromDescription()
{
    TArray<FConstStructView> ConstructedFragments;

    const uint32 NumberOfFragments = this->SpawnFragments.Num();

    for (uint32 FragmentIndex = 0; FragmentIndex < NumberOfFragments; FragmentIndex++) {
        ConstructedFragments.Add(GetSpawnFragmentFromDescription(FragmentIndex, *this));
    }

    return ConstructedFragments;
}

void FSpawnEntityDescription::AddAllFragmentsFromDescriptionToEntity(FMassEntityHandle Entity, FMassExecutionContext& Context)
{
    for (int32 FragmentsIndex = 0; FragmentsIndex < SpawnFragments.Num(); ++FragmentsIndex) {
        switch (SpawnFragments[FragmentsIndex]) {
            case (uint32)(ESpawnFragments::Spawn) : {
                Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FSSpawnFragment>>(Entity, CreateSpawnFragment(*this));
                break;
            }
        }
    }
}

FSpawnEntityDescription::FSpawnEntityDescription()
{
    // Default data
    SpawnFragmentData = MakeTuple(FVector2D());

    SpawnTags = TArray<uint32>();
    SpawnFragments = TArray<uint32>();
}

FSpawnEntityDescription::~FSpawnEntityDescription()
{
    SpawnTags.Empty();
    SpawnFragments.Empty();
}

FVector2D FSpawnEntityDescription::GetFragmentDataSpawn(FSpawnEntityDescription EntityDescription)
{
    return EntityDescription.SpawnFragmentData.Get<0>();
}

void FSpawnEntityDescription::AddFragmentDataSpawn(FVector2D SpawnPositionData, FSpawnEntityDescription& EntityDescription)
{
    if (EntityDescription.SpawnFragments.Num() == 0) {
        EntityDescription.SpawnFragments.Add(uint32(ESpawnFragments::Spawn));
    }
    else {
        if (EntityDescription.SpawnFragments.Find(uint32(ESpawnFragments::Spawn)) == INDEX_NONE) {
            EntityDescription.SpawnFragments.Add(uint32(ESpawnFragments::Spawn));
        }
    }
    EntityDescription.SpawnFragmentData = MakeTuple(SpawnPositionData);
}

bool FSpawnEntityDescription::AddSpawnFragmentToDescription(std::variant<FVector2D> SpawnFragmentData, ESpawnFragments SpawnFragmentEnum, FSpawnEntityDescription& EntityDescription)
{
    if (SpawnFragmentEnum < ESpawnFragments::SpawnFragments) {
        switch (SpawnFragmentEnum) {
        case (ESpawnFragments::Spawn): {
            if (std::holds_alternative<FVector2D>(SpawnFragmentData)) {
                AddFragmentDataSpawn(std::get<FVector2D>(SpawnFragmentData), EntityDescription);
                return true;
            }
        }
        }
    }
    return false;
}

FConstStructView FSpawnEntityDescription::CreateFragmentSpawn(FSpawnEntityDescription EntityDescription)
{
    if (GetFragmentDataSpawn(EntityDescription) != FVector2D(0)) {
        return FConstStructView::Make(FSSpawnFragment(GetFragmentDataSpawn(EntityDescription)));
    }

    return FConstStructView::Make(FSSpawnFragment());
}

FSSpawnFragment FSpawnEntityDescription::CreateSpawnFragment(FSpawnEntityDescription EntityDescription)
{
    return FSSpawnFragment(GetFragmentDataSpawn(EntityDescription));
}
