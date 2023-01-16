

#include "EntityDescriptionMessage.h"

void FEntityDescriptionMessage::AddEntityDescription(EEntityDescriptions EntityDescriptionTypeEnum, FIEntityDescription* EntityDescription)
{
    if (EntityDescriptionEnums.Num() == 0) {
        EntityDescriptionEnums.Add((uint32)EntityDescriptionTypeEnum);
        EntityDescriptions.Add(EntityDescription);
    }
    else {
        if (EntityDescriptionEnums.Find((uint32)EntityDescriptionTypeEnum) == INDEX_NONE) {
            EntityDescriptionEnums.Add((uint32)EntityDescriptionTypeEnum);
            EntityDescriptions.Add(EntityDescription);
        }
    }
}

int32 FEntityDescriptionMessage::FindEntityDescriptionType(EEntityDescriptions EntityDescriptionTypeEnum)
{   
    return EntityDescriptionEnums.Find((uint32)EntityDescriptionTypeEnum);
}

FIEntityDescription* FEntityDescriptionMessage::GetEntityDescription(uint32 IndexOfDescription)
{
    return EntityDescriptions[IndexOfDescription];
}
