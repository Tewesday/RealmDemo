

#include "ActorEntityDescription.h"

FActorEntityDescription::FActorEntityDescription()
{
	// Default data
	ActorFragmentData = MakeTuple(nullptr, 0);

	ActorTags = TArray<uint32>();
	ActorFragments = TArray<uint32>();
}

FActorEntityDescription::~FActorEntityDescription()
{
	ActorTags.Empty();
	ActorFragments.Empty();
}

AActor* FActorEntityDescription::GetFragmentDataActor(FActorEntityDescription EntityDescription)
{
	return EntityDescription.ActorFragmentData.Get<0>();
}

void FActorEntityDescription::AddFragmentDataActor(AActor* ActorData, FActorEntityDescription& EntityDescription)
{
	if (EntityDescription.ActorFragments.Num() == 0) {
		EntityDescription.ActorFragments.Add(uint32(EEntityActorFragments::Actor));
	}
	else {
		if (EntityDescription.ActorFragments.Find(uint32(EEntityActorFragments::Actor)) == INDEX_NONE) {
			EntityDescription.ActorFragments.Add(uint32(EEntityActorFragments::Actor));
		}
	}
	EntityDescription.ActorFragmentData = MakeTuple(ActorData, GetFragmentDataMesh(EntityDescription));
}

FConstStructView FActorEntityDescription::CreateFragmentActor(FActorEntityDescription EntityDescription)
{
	if (GetFragmentDataActor(EntityDescription) != nullptr) {
		return FConstStructView::Make(FAActorFragment(GetFragmentDataActor(EntityDescription)));
	}

	return FConstStructView::Make(FAActorFragment());
}

FAActorFragment FActorEntityDescription::CreateActorFragment(FActorEntityDescription EntityDescription)
{
	return FAActorFragment(GetFragmentDataActor(EntityDescription));
}

uint32 FActorEntityDescription::GetFragmentDataMesh(FActorEntityDescription EntityDescription)
{
	return EntityDescription.ActorFragmentData.Get<1>();
}

void FActorEntityDescription::AddFragmentDataMesh(MeshTypes Mesh, FActorEntityDescription& EntityDescription)
{
	if (EntityDescription.ActorFragments.Num() == 0) {
		EntityDescription.ActorFragments.Add(uint32(EEntityActorFragments::Mesh));
	}
	else {
		if (EntityDescription.ActorFragments.Find(uint32(EEntityActorFragments::Mesh)) == INDEX_NONE) {
			EntityDescription.ActorFragments.Add(uint32(EEntityActorFragments::Mesh));
		}
	}

	EntityDescription.ActorFragmentData = MakeTuple(GetFragmentDataActor(EntityDescription), Mesh);
}

FAMeshFragment FActorEntityDescription::CreateMeshFragment(FActorEntityDescription EntityDescription)
{
	return FAMeshFragment(GetFragmentDataMesh(EntityDescription));
}

bool FActorEntityDescription::AddActorFragmentToDescription(std::variant<AActor*, uint32> ActorFragmentData, EEntityActorFragments FragmentEnum, FActorEntityDescription* EntityDescription)
{
	if (FragmentEnum < EEntityActorFragments::EntityActorFragments) {
		switch (FragmentEnum) {
		case (EEntityActorFragments::Actor): {
			if (std::holds_alternative<AActor*>(ActorFragmentData)) {
				AddFragmentDataActor(std::get<AActor*>(ActorFragmentData), *EntityDescription);
				return true;
			}
		}
		case (EEntityActorFragments::Mesh): {
			if (std::holds_alternative<uint32>(ActorFragmentData)) {
				AddFragmentDataMesh((MeshTypes)std::get<uint32>(ActorFragmentData), *EntityDescription);
				return true;
			}
		}
		}
	}
	return false;
}

FConstStructView FActorEntityDescription::GetActorFragmentFromDescription(uint32 FragmentIndex, FActorEntityDescription EntityDescription)
{
	switch (EntityDescription.ActorFragments[FragmentIndex]) {
		case (uint32)(EEntityActorFragments::Actor) : {
			return CreateFragmentActor(EntityDescription);
		}
		case (uint32)(EEntityActorFragments::Mesh) : {
			//return CreateFragmentMesh(EntityDescription);
			return FConstStructView();
		}
	}
	return FConstStructView();
}

TArray<FConstStructView> FActorEntityDescription::GetAllFragmentsFromDescription()
{
	TArray<FConstStructView> ConstructedFragments;

	const uint32 NumberOfFragments = this->ActorFragments.Num();

	for (uint32 FragmentIndex = 0; FragmentIndex < NumberOfFragments; FragmentIndex++) {
		ConstructedFragments.Add(GetActorFragmentFromDescription(FragmentIndex, *this));
	}

	return ConstructedFragments;
}

void FActorEntityDescription::AddAllFragmentsFromDescriptionToEntity(FMassEntityHandle Entity, FMassExecutionContext& Context)
{
	for (int32 FragmentsIndex = 0; FragmentsIndex < ActorFragments.Num(); ++FragmentsIndex) {
		switch (ActorFragments[FragmentsIndex]) {
			case (uint32)(EEntityActorFragments::Actor) : {
				Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FAActorFragment>>(Entity, CreateActorFragment(*this));
				break;
			}
			case (uint32)(EEntityActorFragments::Mesh) : {
				Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FAMeshFragment>>(Entity, CreateMeshFragment(*this));
				break;
			}
		}
	}
}
