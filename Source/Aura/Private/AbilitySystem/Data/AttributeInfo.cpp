
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Aura/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
    for (const FAuraAttributeInfo& Info : AttributeInformation)
    {
        if (Info.AttributeTag.MatchesTagExact(AttributeTag))
        {
            return Info;
        }
    }

    if (bLogNotFound)
    {
        UE_LOG(LogAura, Error, TEXT("AbilityTag [%s]에 해당하는 [%s] AttributeInfo [%s]를 찾을 수 없습니다."), *AttributeTag.ToString(), *GetNameSafe(this));
    }

    return FAuraAttributeInfo();
}
