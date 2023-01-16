

#include "MovementEntityDescription.h"

FMovementEntityDescription::FMovementEntityDescription()
{
	// Default init tuple data
	MovementFragmentData = MakeTuple(FTransform(), 0.0, FVector2D(), FVector2D());
	MovementTags = TArray<uint32>();
	MovementFragments = TArray<uint32>();
}

FMovementEntityDescription::~FMovementEntityDescription()
{
	MovementTags.Empty();
	MovementFragments.Empty();
}

FTransform FMovementEntityDescription::GetFragmentDataTransform(FMovementEntityDescription EntityDescription)
{
	return EntityDescription.MovementFragmentData.Get<0>();
}

void FMovementEntityDescription::AddFragmentDataTransform(FTransform TransformData, FMovementEntityDescription& EntityDescription)
{
	if (EntityDescription.MovementFragments.Num() == 0) {
		EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Transform));
	}
	else {
		if (EntityDescription.MovementFragments.Find(uint32(EMovementFragments::Transform)) == INDEX_NONE) {
			EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Transform));
		}
	}
	EntityDescription.MovementFragmentData = MakeTuple(TransformData, GetFragmentDataAgentRadius(EntityDescription), GetFragmentDataSteeringStartPos(EntityDescription), GetFragmentDataSteeringMaxVelocity(EntityDescription));
}

FConstStructView FMovementEntityDescription::CreateFragmentTransform(FMovementEntityDescription EntityDescription)
{
	if (!FTransform().Equals(GetFragmentDataTransform(EntityDescription))) {
		return FConstStructView::Make(FMTransformFragment(GetFragmentDataTransform(EntityDescription)));
	}

	return FConstStructView::Make(FMTransformFragment());
}

FMTransformFragment FMovementEntityDescription::CreateTransformFragment(FMovementEntityDescription EntityDescription)
{
	return FMTransformFragment(GetFragmentDataTransform(EntityDescription));
}

float FMovementEntityDescription::GetFragmentDataAgentRadius(FMovementEntityDescription EntityDescription)
{
	return EntityDescription.MovementFragmentData.Get<1>();
}

void FMovementEntityDescription::AddFragmentDataAgentRadius(float AgentRadiusData, FMovementEntityDescription& EntityDescription)
{
	if (EntityDescription.MovementFragments.Num() == 0) {
		EntityDescription.MovementFragments.Add(uint32(EMovementFragments::AgentRadius));
	}
	else {
		if (EntityDescription.MovementFragments.Find(uint32(EMovementFragments::AgentRadius)) == INDEX_NONE) {
			EntityDescription.MovementFragments.Add(uint32(EMovementFragments::AgentRadius));
		}
	}
	EntityDescription.MovementFragmentData = MakeTuple(GetFragmentDataTransform(EntityDescription), AgentRadiusData, GetFragmentDataSteeringStartPos(EntityDescription), GetFragmentDataSteeringMaxVelocity(EntityDescription));
}

FConstStructView FMovementEntityDescription::CreateFragmentAgentRadius(FMovementEntityDescription EntityDescription)
{
	if (FMAgentRadiusFragment::DEFAULT_RADIUS != GetFragmentDataAgentRadius(EntityDescription)) {
		return FConstStructView::Make(FMAgentRadiusFragment(GetFragmentDataAgentRadius(EntityDescription)));
	}

	return FConstStructView::Make(FMAgentRadiusFragment());
}

FMAgentRadiusFragment FMovementEntityDescription::CreateAgentRadiusFragment(FMovementEntityDescription EntityDescription)
{
	return FMAgentRadiusFragment(GetFragmentDataAgentRadius(EntityDescription));
}

FVector2D FMovementEntityDescription::GetFragmentDataSteeringStartPos(FMovementEntityDescription EntityDescription)
{
	return EntityDescription.MovementFragmentData.Get<2>();
}

void FMovementEntityDescription::AddFragmentDataSteeringStartPos(FVector2D SteeringStartPosData, FMovementEntityDescription& EntityDescription)
{
	if (EntityDescription.MovementFragments.Num() == 0) {
		EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Steering));
	}
	else {
		if (EntityDescription.MovementFragments.Find(uint32(EMovementFragments::Steering)) == INDEX_NONE) {
			EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Steering));
		}
	}
	EntityDescription.MovementFragmentData = MakeTuple(GetFragmentDataTransform(EntityDescription), GetFragmentDataAgentRadius(EntityDescription), SteeringStartPosData, GetFragmentDataSteeringMaxVelocity(EntityDescription));
}

FVector2D FMovementEntityDescription::GetFragmentDataSteeringMaxVelocity(FMovementEntityDescription EntityDescription)
{
	return EntityDescription.MovementFragmentData.Get<3>();
}

void FMovementEntityDescription::AddFragmentDataSteeringMaxVelocity(FVector2D SteeringMaxVelocityData, FMovementEntityDescription& EntityDescription)
{
	if (EntityDescription.MovementFragments.Num() == 0) {
		EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Steering));
	}
	else {
		if (EntityDescription.MovementFragments.Find(uint32(EMovementFragments::Steering)) == INDEX_NONE) {
			EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Steering));
		}
	}

	EntityDescription.MovementFragmentData = MakeTuple(GetFragmentDataTransform(EntityDescription), GetFragmentDataAgentRadius(EntityDescription), GetFragmentDataSteeringStartPos(EntityDescription), SteeringMaxVelocityData);
}

FConstStructView FMovementEntityDescription::CreateFragmentSteering(FMovementEntityDescription EntityDescription)
{
	if (GetFragmentDataSteeringStartPos(EntityDescription) != FMSteeringFragment::DEFAULT_POSITION) {
		if (GetFragmentDataSteeringMaxVelocity(EntityDescription) != FMSteeringFragment::DEFAULT_MAX_VELOCITY) {
			return FConstStructView::Make(FMSteeringFragment(GetFragmentDataSteeringStartPos(EntityDescription), GetFragmentDataSteeringMaxVelocity(EntityDescription)));
		}
		return FConstStructView::Make(FMSteeringFragment(GetFragmentDataSteeringStartPos(EntityDescription)));
	}

	if (GetFragmentDataSteeringMaxVelocity(EntityDescription) != FMSteeringFragment::DEFAULT_MAX_VELOCITY) {
		return FConstStructView::Make(FMSteeringFragment(FMSteeringFragment::DEFAULT_POSITION, GetFragmentDataSteeringMaxVelocity(EntityDescription)));
	}

	return FConstStructView::Make(FMSteeringFragment());
}

FMSteeringFragment FMovementEntityDescription::CreateSteeringFragment(FMovementEntityDescription EntityDescription)
{
	// TODO: handle possible missing data
	return FMSteeringFragment(GetFragmentDataSteeringStartPos(EntityDescription), GetFragmentDataSteeringMaxVelocity(EntityDescription));
}

void FMovementEntityDescription::AddFragmentOrder(FMovementEntityDescription& EntityDescription)
{
	if (EntityDescription.MovementFragments.Num() == 0) {
		EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Order));
	}
	else {
		if (EntityDescription.MovementFragments.Find(uint32(EMovementFragments::Order)) == INDEX_NONE) {
			EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Order));
		}
	}
}

FMOrderFragment FMovementEntityDescription::CreateOrderFragment(FMovementEntityDescription EntityDescription)
{
	return FMOrderFragment();
}

void FMovementEntityDescription::AddFragmentOctree(FMovementEntityDescription& EntityDescription)
{
	if (EntityDescription.MovementFragments.Num() == 0) {
		EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Octree));
	}
	else {
		if (EntityDescription.MovementFragments.Find(uint32(EMovementFragments::Octree)) == INDEX_NONE) {
			EntityDescription.MovementFragments.Add(uint32(EMovementFragments::Octree));
		}
	}
}

FMOctreeFragment FMovementEntityDescription::CreateOctreeFragment(FMovementEntityDescription EntityDescription)
{
	return FMOctreeFragment();
}

void FMovementEntityDescription::AddFragmentFormationMember(FMovementEntityDescription& EntityDescription)
{
	if (EntityDescription.MovementFragments.Num() == 0) {
		EntityDescription.MovementFragments.Add(uint32(EMovementFragments::FormationMember));
	}
	else {
		if (EntityDescription.MovementFragments.Find(uint32(EMovementFragments::FormationMember)) == INDEX_NONE) {
			EntityDescription.MovementFragments.Add(uint32(EMovementFragments::FormationMember));
		}
	}
}

FMFormationMemberFragment FMovementEntityDescription::CreateFormationMemberFragment(FMovementEntityDescription EntityDescription)
{
	return FMFormationMemberFragment();
}

bool FMovementEntityDescription::AddMovementFragmentToDescription(std::variant<FTransform, float, FVector2D> MovementFragmentData, EDataArgument DataArgument, EMovementFragments FragmentEnum, FMovementEntityDescription* EntityDescription)
{
	if (FragmentEnum < EMovementFragments::MovementFragments) {
		switch (DataArgument) {
		case (EDataArgument::First): {
			return AddMovementFragmentToDescriptionFirstArgument(MovementFragmentData, FragmentEnum, EntityDescription);
		}
		case (EDataArgument::Second): {
		}
			return AddMovementFragmentToDescriptionSecondArgument(MovementFragmentData, FragmentEnum, EntityDescription);
		}
	}

	return false;
}

bool FMovementEntityDescription::AddMovementFragmentToDescriptionFirstArgument(std::variant<FTransform, float, FVector2D> MovementFragmentData, EMovementFragments FragmentEnum, FMovementEntityDescription* EntityDescription)
{
	switch (FragmentEnum) {
	case (EMovementFragments::Transform): {
		if (std::holds_alternative<FTransform>(MovementFragmentData)) {
			AddFragmentDataTransform(std::get<FTransform>(MovementFragmentData), *EntityDescription);
			return true;
		}
	}
	case (EMovementFragments::AgentRadius): {
		if (std::holds_alternative<float>(MovementFragmentData)) {
			AddFragmentDataAgentRadius(std::get<float>(MovementFragmentData), *EntityDescription);
			return true;
		}
	}
	case (EMovementFragments::Steering): {
		if (std::holds_alternative<FVector2D>(MovementFragmentData)) {
			AddFragmentDataSteeringStartPos(std::get<FVector2D>(MovementFragmentData), *EntityDescription);
			return true;
		}
	}
	case (EMovementFragments::Order): {
		AddFragmentOrder(*EntityDescription);
		return true;
	}
	case (EMovementFragments::Octree): {
		AddFragmentOctree(*EntityDescription);
		return true;
	}
	case (EMovementFragments::FormationMember): {
		AddFragmentFormationMember(*EntityDescription);
		return true;
	}
	}

	return false;
}

bool FMovementEntityDescription::AddMovementFragmentToDescriptionSecondArgument(std::variant<FTransform, float, FVector2D> MovementFragmentData, EMovementFragments FragmentEnum, FMovementEntityDescription* EntityDescription)
{
	switch (FragmentEnum) {
	case (EMovementFragments::Steering): {
		if (std::holds_alternative<FVector2D>(MovementFragmentData)) {
			AddFragmentDataSteeringMaxVelocity(std::get<FVector2D>(MovementFragmentData), *EntityDescription);
			return true;
		}
	}
	}

	return false;
}

FConstStructView FMovementEntityDescription::GetMovementFragmentFromDescription(uint32 FragmentIndex, FMovementEntityDescription EntityDescription)
{
	switch (EntityDescription.MovementFragments[FragmentIndex]) {
		case (uint32)(EMovementFragments::Transform) : {
			return CreateFragmentTransform(EntityDescription);
		}
		case (uint32)(EMovementFragments::AgentRadius) : {
			return CreateFragmentAgentRadius(EntityDescription);
		}
		case (uint32)(EMovementFragments::Steering) : {
			return CreateFragmentSteering(EntityDescription);
		}
	}
	return FConstStructView();
}

TArray<FConstStructView> FMovementEntityDescription::GetAllFragmentsFromDescription()
{
	TArray<FConstStructView> ConstructedFragments;

	const uint32 NumberOfFragments = this->MovementFragments.Num();

	for (uint32 FragmentIndex = 0; FragmentIndex < NumberOfFragments; FragmentIndex++) {
		ConstructedFragments.Add(GetMovementFragmentFromDescription(FragmentIndex, *this));
	}

	return ConstructedFragments;
}

void FMovementEntityDescription::AddAllFragmentsFromDescriptionToEntity(FMassEntityHandle Entity, FMassExecutionContext& Context)
{
	for (int32 FragmentsIndex = 0; FragmentsIndex < MovementFragments.Num(); ++FragmentsIndex) {
		switch ((uint32)MovementFragments[FragmentsIndex]) {
			case (uint32)(EMovementFragments::Transform) : {
				Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FMTransformFragment>>(Entity, CreateTransformFragment(*this));
				break;
			}
			case (uint32)(EMovementFragments::AgentRadius) : {
				Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FMAgentRadiusFragment>>(Entity, CreateAgentRadiusFragment(*this));
				break;
			}
			case (uint32)(EMovementFragments::Steering) : {
				Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FMSteeringFragment>>(Entity, CreateSteeringFragment(*this));
				break;
			}
			case (uint32)(EMovementFragments::Order) : {
				Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FMOrderFragment>>(Entity, CreateOrderFragment(*this));
				break;
			}
			case (uint32)(EMovementFragments::Octree) : {
				Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FMOctreeFragment>>(Entity, CreateOctreeFragment(*this));
				break;
			}
			case (uint32)(EMovementFragments::FormationMember) : {
				Context.Defer().PushCommand<FMassCommandAddFragmentInstances<FMFormationMemberFragment>>(Entity, CreateFormationMemberFragment(*this));
				break;
			}
		}
	}
}
