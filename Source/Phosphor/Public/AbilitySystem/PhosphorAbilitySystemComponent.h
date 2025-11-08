// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PhosphorAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&)

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpPassiveAbilities);
	
	bool bStartupAbilitiesGiven=false;

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& Spec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& Spec);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server,Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
protected:

	virtual void OnRep_ActivateAbilities() override;
	
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent
		, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
