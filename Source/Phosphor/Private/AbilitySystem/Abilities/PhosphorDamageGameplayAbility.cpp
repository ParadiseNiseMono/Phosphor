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

FDamageEffectParams UPhosphorDamageGameplayAbility::MakeDamageEffectParamsFromDefaults(AActor* TargetActor) const
{
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContext = GetAvatarActorFromActorInfo();
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DebuffChance = DebuffChance.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.DebuffDamage = DebuffDamage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.DebuffDuration = DebuffDuration.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.DebuffFrequency = DebuffFrequency.GetValueAtLevel(GetAbilityLevel());

	return DamageEffectParams;
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
