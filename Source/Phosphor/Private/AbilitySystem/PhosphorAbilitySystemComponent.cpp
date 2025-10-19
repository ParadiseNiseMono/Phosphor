// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAbilitySystemComponent.h"

#include "PhosphorGameplayTags.h"

void UPhosphorAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UPhosphorAbilitySystemComponent::EffectApplied);

}

void UPhosphorAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                    const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);

}
