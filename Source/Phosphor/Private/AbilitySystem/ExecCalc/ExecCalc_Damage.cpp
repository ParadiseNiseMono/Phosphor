// copyright Paradise_NiseMono


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct PhosphorDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	PhosphorDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,CriticalHitDamage,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,CriticalHitResistance,Target,false);
	}

	
};

static const PhosphorDamageStatics& DamageStatics()
{
	static PhosphorDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargerASC=ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar=SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar=TargerASC ? TargerASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterFace=Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterFace=Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec&	Spec=ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags=Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=SourceTags;
	EvaluateParameters.TargetTags=TargetTags;

	const UCharacterClassInfo* CharacterClassInfo=UPhosphorAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	//Get Damage set  by Caller Magnitude
	float Damage=Spec.GetSetByCallerMagnitude(FPhosphorGameplayTags::Get().Damage);

	float SourceCriticalHitChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParameters,SourceCriticalHitChance);
	SourceCriticalHitChance=FMath::Max(SourceCriticalHitChance,0.f);
	
	float SourceCriticalHitDamage=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluateParameters,SourceCriticalHitDamage);
	SourceCriticalHitDamage=FMath::Max(SourceCriticalHitDamage,0.f);

	float TargetCriticalHitResistance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,EvaluateParameters,TargetCriticalHitResistance);
	TargetCriticalHitResistance=FMath::Max(TargetCriticalHitResistance,0.f);

	//If critical,double the damage.
	
	
	const FRealCurve* CriticalHitResistanceCurve=CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("CriticalHitResistance"),FString(""));
	const float CriticalHitResistanceCoefficient= CriticalHitResistanceCurve->Eval(TargetCombatInterFace->GetPlayerLevel());
	
	const float EffectiveCriticalHitChance=SourceCriticalHitChance*(100-CriticalHitResistanceCoefficient*TargetCriticalHitResistance)/100.f;
	const bool bCritical=FMath::FRandRange(1.f,100.f)<EffectiveCriticalHitChance;
	Damage=bCritical ? Damage*2+SourceCriticalHitDamage : Damage;

	//Capture BlockChance on Target,and determine if there was a successful block.

	float TargetBlockChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParameters,TargetBlockChance);
	TargetBlockChance=FMath::Max(TargetBlockChance,0.f);

	//If block,halve the damage.
	const bool bBlocked=FMath::FRandRange(1.f,100.f)<TargetBlockChance;
	Damage=bBlocked ? Damage/2 : Damage;

	float TargetArmor=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,TargetArmor);
	TargetArmor=FMath::Max(TargetArmor,0.f);

	float SourceArmorPenetration=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParameters,SourceArmorPenetration);
	SourceArmorPenetration=FMath::Max(SourceArmorPenetration,0.f);
	
	const FRealCurve* ArmorPenetrationCurve=CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("ArmorPenetration"),FString(""));
	const float ArmorPenetrationCoefficient= ArmorPenetrationCurve->Eval(SourceCombatInterFace->GetPlayerLevel());
	
	//ArmorPenetration ignores percentage of the Target's Armor
	const float EffectiveArmor=TargetArmor*((100-SourceArmorPenetration * ArmorPenetrationCoefficient)/100.f);

	const FRealCurve* EffectArmorCurve=CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmor"),FString(""));
	const float EffectArmorCoefficient=EffectArmorCurve->Eval(TargetCombatInterFace->GetPlayerLevel());
	//Armor ignore a percentage of incoming damage.
	Damage *=(100 - EffectiveArmor * EffectArmorCoefficient)/100.f;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UPhosphorAttributeSet::GetInComingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
