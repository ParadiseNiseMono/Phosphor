// copyright Paradise_NiseMono


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Phosphor/PhosphorLogChannels.h"

FPhosphorAbilityInfo UAbilityInfo::FindAbilityByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FPhosphorAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogPhosphor, Error, TEXT("Can't find ability tag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	
	return FPhosphorAbilityInfo();
}
