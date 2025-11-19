// copyright Paradise_NiseMono


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "PhosphorAbilityTypes.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Phosphor/PhosphorLogChannels.h"

struct PhosphorDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightingResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	
	PhosphorDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,CriticalHitDamage,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,CriticalHitResistance,Target,false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,LightingResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPhosphorAttributeSet,PhysicalResistance,Target,false);

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

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightingResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec,
	FAggregatorEvaluateParameters EvaluateParameters, const TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FPhosphorGameplayTags& GameplayTags = FPhosphorGameplayTags::Get();

	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1);
		if (TypeDamage > 1.f)
		{
			//Determine if there is a successful Debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluateParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100.f - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UPhosphorAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UPhosphorAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1);

				UPhosphorAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UPhosphorAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UPhosphorAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FPhosphorGameplayTags& Tags=FPhosphorGameplayTags::Get();
		
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
		
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightingResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
	
	const UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar=SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar=TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel=1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel=1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags=SourceTags;
	EvaluateParameters.TargetTags=TargetTags;

	const UCharacterClassInfo* CharacterClassInfo=UPhosphorAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	//Debuff
	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters, TagsToCaptureDefs);

	//Get Damage set  by Caller Magnitude
	float Damage=0;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair:FPhosphorGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(TagsToCaptureDefs.Contains(ResistanceTag),TEXT("TagToCaptureDefs doesn't contain Tag:[%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition,EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		
		DamageTypeValue *= (100.f-Resistance) / 100.f;

		if (UPhosphorAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageSignature().AddLambda([&](float DamageAmount)
				{
					DamageTypeValue = DamageAmount;
				});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(TargetAvatar,
				DamageTypeValue,
				0.f,
				UPhosphorAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UPhosphorAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UPhosphorAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar);
		}
		
		Damage += DamageTypeValue;
	}

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
	const float CriticalHitResistanceCoefficient= CriticalHitResistanceCurve->Eval(TargetPlayerLevel);
	
	const float EffectiveCriticalHitChance=SourceCriticalHitChance*(100-CriticalHitResistanceCoefficient*TargetCriticalHitResistance)/100.f;
	const bool bCritical=FMath::FRandRange(1.f,100.f)<EffectiveCriticalHitChance;
	Damage=bCritical ? Damage*2+SourceCriticalHitDamage : Damage;
	
	UPhosphorAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,bCritical);
	

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

	UPhosphorAbilitySystemLibrary::SetIsBlockHit(EffectContextHandle,bBlocked);
	
	const FRealCurve* ArmorPenetrationCurve=CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("ArmorPenetration"),FString(""));
	const float ArmorPenetrationCoefficient= ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	
	//ArmorPenetration ignores percentage of the Target's Armor
	const float EffectiveArmor=TargetArmor*((100-SourceArmorPenetration * ArmorPenetrationCoefficient)/100.f);

	const FRealCurve* EffectArmorCurve=CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmor"),FString(""));
	const float EffectArmorCoefficient=EffectArmorCurve->Eval(TargetPlayerLevel);
	//Armor ignore a percentage of incoming damage.
	Damage *=(100 - EffectiveArmor * EffectArmorCoefficient)/100.f;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UPhosphorAttributeSet::GetInComingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
