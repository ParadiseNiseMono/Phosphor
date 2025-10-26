// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAbilitySystemGlobals.h"

#include "PhosphorAbilityTypes.h"

FGameplayEffectContext* UPhosphorAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FPhosphorGameplayEffectContext();
}
