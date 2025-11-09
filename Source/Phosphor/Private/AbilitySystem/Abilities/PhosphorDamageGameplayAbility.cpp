// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Fonts/UnicodeBlockRange.h"
#include "Interaction/CombatInterface.h"


void UPhosphorDamageGameplayAbility::CauseDamage(AActor* Target)
{
	FGameplayEffectSpecHandle DamageSpecHandle=MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	for (TTuple<FGameplayTag,FScalableFloat> Pair:DamageTypes)
	{
		const float ScaledDamage=Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key,ScaledDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target));
}

FTaggedMontage UPhosphorDamageGameplayAbility::GetRandomTaggedMontageFromArray(
	const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num()>0)
	{
		const int32 RandomIndex=FMath::RandRange(0,TaggedMontages.Num()-1);
		return TaggedMontages[RandomIndex];
	}
	return FTaggedMontage();
}

float UPhosphorDamageGameplayAbility::GetDamageByDamageType(int32 InLevel, const FGameplayTag& DamageType)
{
	checkf(DamageTypes.Contains(DamageType), TEXT("Gameplay Ability [%s] does not contain Damage Type [%s]"), *GetNameSafe(this), *DamageType.ToString());
	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
}
