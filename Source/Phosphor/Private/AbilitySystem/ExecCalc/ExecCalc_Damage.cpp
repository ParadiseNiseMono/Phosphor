// copyright Paradise_NiseMono


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAttributeSet.h"

struct PhosphorDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	PhosphorDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,BlockChance,Target,false);
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
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargerASC=ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar=SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar=TargerASC ? TargerASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec&	Spec=ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags=Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags=Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=SourceTags;
	EvaluateParameters.TargetTags=TargetTags;

	//Get Damage set  by Caller Magnitude
	float Damage=Spec.GetSetByCallerMagnitude(FPhosphorGameplayTags::Get().Damage);

	//Capture BlockChance on Target,and determine if there was a successful block.
	//If block,halve the damage.
	float TargetBlockChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParameters,TargetBlockChance);
	TargetBlockChance=FMath::Max(TargetBlockChance,0.f);

	const bool bBlocked=FMath::FRandRange(1.f,100.f)<TargetBlockChance;
	Damage=bBlocked ? Damage/2 : Damage;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UPhosphorAttributeSet::GetInComingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
