// copyright Paradise_NiseMono


#include "Input/PhosphorInputConfig.h"

const UInputAction* UPhosphorInputConfig::FindAbilityInputForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FPhosphorInputAction& Action : InputActions)
	{
		if (Action.InputAction&&Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find info for AbilityTag [%s] on InputAction [%s]."),*InputTag.ToString(),*GetNameSafe(this));
	}
	return nullptr;
}
