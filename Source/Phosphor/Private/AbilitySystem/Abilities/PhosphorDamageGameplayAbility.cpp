// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Fonts/UnicodeBlockRange.h"
#include "Interaction/CombatInterface.h"


void UPhosphorDamageGameplayAbility::CauseDamage(AActor* Target)
{
	FGameplayEffectSpecHandle DamageSpecHandle=MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	const float ScaledDamage=Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,DamageType,ScaledDamage);
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
