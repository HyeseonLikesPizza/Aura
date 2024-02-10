
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("AbilityTag [%s]에 해당하는 AbilityInfo [%s]를 찾을 수 없습니다."), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAbilityInfo();
}
