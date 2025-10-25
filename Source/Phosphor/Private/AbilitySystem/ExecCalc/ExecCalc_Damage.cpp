// copyright Paradise_NiseMono


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAttributeSet.h"

struct PhosphorDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	
	PhosphorDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,Armor,Target,false);
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

	float Armor=0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,Armor);
	Armor=FMath::Max<float>(0.0f,Armor);
	++Armor;

	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty,EGameplayModOp::Additive,Armor);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
