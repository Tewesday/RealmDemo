


#include "EntityActorPrc.h"


UEntityActorPrc::UEntityActorPrc()
{
    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
    ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UEntityActorPrc::Initialize(UObject& Owner)
{
}

void UEntityActorPrc::ConfigureQueries()
{
    ActorEntityQuery.AddRequirement<FAActorFragment>(EMassFragmentAccess::ReadWrite);
    ActorEntityQuery.RegisterWithProcessor(*this);
}

void UEntityActorPrc::Execute(FMassEntityManager& EntitySubsystem, FMassExecutionContext& Context)
{
    if (SimTimeAuthoritySys == nullptr) {
        SimTimeAuthoritySys = GetWorld()->GetGameInstance()->GetSubsystem<USimTimeAuthoritySubsystem>();
    }

    if (EntityActorSys == nullptr) {
        EntityActorSys = GetWorld()->GetSubsystem<UEntityActorSys>();
    }

    if (EntityDescriptionSys == nullptr) {
        EntityDescriptionSys = GetWorld()->GetSubsystem<UEntityDescriptionSys>();
    }

    // Create Entity/Actors bypassing SpawnSys/Prc
    int32 MessagesCount = EntityDescriptionSys->EntityDescriptionMessages.Num();
    for (int32 MessageIndex = 0; MessageIndex < MessagesCount; ++MessageIndex) {
        FEntityDescriptionMessage Message = EntityDescriptionSys->EntityDescriptionMessages[MessageIndex];
        AActor* Actor = EntityActorSys->SpawnActor();
        FMassEntityHandle Entity = EntityActorSys->SpawnEntity(Actor);

        for (FIEntityDescription* EntityDescription : EntityDescriptionSys->EntityDescriptionMessages[MessageIndex].EntityDescriptions) {
            EntityDescription->AddAllFragmentsFromDescriptionToEntity(Entity, Context);
        }
    }
    // All messages should have been completed
    EntityDescriptionSys->EntityDescriptionMessages.Empty();


    ActorEntityQuery.ForEachEntityChunk(EntitySubsystem, Context,
        [&, this](FMassExecutionContext& Context) {
            const TArrayView<FAActorFragment> EntityActorFragments = Context.GetMutableFragmentView<FAActorFragment>();

            // Non Tick Based processing
            const int32 QueryLength = Context.GetNumEntities();

            for (int32 i = 0; i < QueryLength; ++i) {

                // Verify Entity has reference to its actor
                if (EntityActorFragments[i].Actor == nullptr) {
                    EntityActorSys->AssignActor(Context.GetEntity(i));
                }
            }

            // Tick Based processing

            if (LastTickCalculated == SimTimeAuthoritySys->SimTimeAuthority.CurrentTick) {
                return;
            }

            for (int32 i = 0; i < QueryLength; ++i) {
            }
        });

    LastTickCalculated = SimTimeAuthoritySys->SimTimeAuthority.CurrentTick;
}