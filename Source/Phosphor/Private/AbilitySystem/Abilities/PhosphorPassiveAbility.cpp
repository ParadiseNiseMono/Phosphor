// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"

void UPhosphorPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UPhosphorAbilitySystemComponent* PhosphorASC = Cast<UPhosphorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		PhosphorASC->DeactivatePassiveAbility.AddUObject(this, &UPhosphorPassiveAbility::ReceiveDeActivate);
	}
}

void UPhosphorPassiveAbility::ReceiveDeActivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
