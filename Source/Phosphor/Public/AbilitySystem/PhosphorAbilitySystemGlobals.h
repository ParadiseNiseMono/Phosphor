// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "PhosphorAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
