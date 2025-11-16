// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorGameplayAbility.h"
#include "PhosphorPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorPassiveAbility : public UPhosphorGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	void ReceiveDeActivate(const FGameplayTag& AbilityTag);
};
