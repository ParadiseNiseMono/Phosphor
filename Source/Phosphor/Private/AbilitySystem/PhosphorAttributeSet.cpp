// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "PhosphorAbilityTypes.h"
#include "GameFramework/Character.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Phosphor/PhosphorLogChannels.h"
#include "Player/PhosphorPlayerController.h"


UPhosphorAttributeSet::UPhosphorAttributeSet()
{
	const FPhosphorGameplayTags& PhosphorGameplayTags=FPhosphorGameplayTags::Get();

	/*Primary Attributes*/
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Primary_Strength,GetStrengthAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Primary_Intelligence,GetIntelligenceAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Primary_Resilience,GetResilienceAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Primary_Vigor,GetVigorAttribute);
	/*Secondary Attributes*/
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_Armor,GetArmorAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_BlockChance,GetBlockChanceAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_CriticalHitResistance,GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_HealthRegeneration,GetHealthRegenerationAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_ManaRegeneration,GetManaRegenerationAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_MaxHealth,GetMaxHealthAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Secondary_MaxMana,GetMaxManaAttribute);
	/*Resistance Attributes*/
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Resistance_Fire,GetFireResistanceAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Resistance_Lightning,GetLightingResistanceAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Resistance_Arcane,GetArcaneResistanceAttribute);
	TagsToAttributes.Add(PhosphorGameplayTags.Attributes_Resistance_Physical,GetPhysicalResistanceAttribute);
}

void UPhosphorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/*primary attributes*/
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Vigor,COND_None,REPNOTIFY_Always);

	/*Secondary Attributes*/
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,ArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,BlockChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,CriticalHitChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,CriticalHitDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,CriticalHitResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,HealthRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,ManaRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);

	/*Resistance Attributes*/
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,FireResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,LightingResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,ArcaneResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,PhysicalResistance,COND_None,REPNOTIFY_Always);

	/*Vital Attributes*/
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Mana,COND_None,REPNOTIFY_Always);
}

void UPhosphorAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute==GetHealthAttribute())
	{
		NewValue=FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}
	if (Attribute==GetManaAttribute())
	{
		NewValue=FMath::Clamp(NewValue,0.f,GetMaxMana());
	}
}

void UPhosphorAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props) const
{
	//Source=causer of the effect,Target=target of this effect (Owner of this ASC)

	Props.EffectContextHandle=Data.EffectSpec.GetContext();
	Props.SourceASC=Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC)&& Props.SourceASC->AbilityActorInfo.IsValid()&&Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor=Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController=Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController==nullptr&&Props.SourceASC!=nullptr)
		{
			if (const APawn* Pawn = Cast<const APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController=Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter=Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid()&&Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor=Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController=Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter=Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
	
}


void UPhosphorAttributeSet::PostGameplayEffectExecute(const  FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data,Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_IsDead(Props.TargetCharacter))
		{
			return;
		}
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		HandleInComingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetInComingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UPhosphorAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth=false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana=false;
	}
	
}

void UPhosphorAttributeSet::ShowFloatText(const FEffectProperties& Props, float Damage,bool bBlockHit,bool bCriticalHit) const
{
	if (Props.SourceCharacter!=Props.TargetCharacter)
	{
		if (APhosphorPlayerController* PC=Cast<APhosphorPlayerController>(Props.SourceCharacter->GetController()))
		{
			PC->ShowDamageNumber(Damage,Props.TargetCharacter,bBlockHit,bCriticalHit);
			return;
		}
		if (APhosphorPlayerController* PC=Cast<APhosphorPlayerController>(Props.TargetCharacter->GetController()))
		{
			PC->ShowDamageNumber(Damage,Props.TargetCharacter,bBlockHit,bCriticalHit);
		}
	}
}

void UPhosphorAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel=ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass=ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UPhosphorAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter,TargetClass,TargetLevel);

		const FPhosphorGameplayTags& GameplayTags=FPhosphorGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag=GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude=XPReward;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter,GameplayTags.Attributes_Meta_IncomingXP,Payload);
	}	
}

void UPhosphorAttributeSet::HandleInComingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetInComingDamage();
	SetInComingDamage(0.f);
	if (LocalIncomingDamage>0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth,0.f,GetMaxHealth()));

		const bool bFatal = NewHealth <= 0;
		if (bFatal)
		{
			ICombatInterface* CombatInterface=Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die(UPhosphorAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			}
			SendXPEvent(Props);
		}
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FPhosphorGameplayTags::Get().Effects_HitReact);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);

			const FVector& KnockbackForce = UPhosphorAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}
		const bool bBlockHit=UPhosphorAbilitySystemLibrary::IsBlockHit(Props.EffectContextHandle);
		const bool bCriticalHit=UPhosphorAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatText(Props,LocalIncomingDamage,bBlockHit,bCriticalHit);

		if (UPhosphorAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
	}
}

void UPhosphorAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP=GetInComingXP();
	SetInComingXP(0.f);

	//TODO: See if we should Level up.
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel=ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP=IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		const int32 NewLevel=IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter,LocalIncomingXP+CurrentXP);\
		const int32 NumLevelUps=NewLevel-CurrentLevel;
		if (NumLevelUps>0)
		{
			const int32 AttributePoints=IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter,CurrentLevel);
			const int32 SpellPoints=IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter,CurrentLevel);

			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter,NumLevelUps);

			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter,AttributePoints);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter,SpellPoints);

			bTopOffHealth=true;
			bTopOffMana=true;
				
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
			
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter,LocalIncomingXP);
	}
}

void UPhosphorAttributeSet::Debuff(const FEffectProperties& Props)
{
	const FPhosphorGameplayTags GameplayTags = FPhosphorGameplayTags::Get();
	FGameplayEffectContextHandle EffectContextHandle = Props.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UPhosphorAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UPhosphorAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UPhosphorAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UPhosphorAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	
	Effect->CachedGrantedTags.AddTag(GameplayTags.DamageTypesToDebuffs[DamageType]);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers.Last();

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UPhosphorAttributeSet::GetInComingDamageAttribute();
	
	
	if (FGameplayEffectSpec* Mutable = new FGameplayEffectSpec(Effect, EffectContextHandle, 1))
	{
		FPhosphorGameplayEffectContext* PhosphorContext = static_cast<FPhosphorGameplayEffectContext*>(EffectContextHandle.Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		PhosphorContext->SetDamageType(DebuffDamageType);
		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*Mutable);
	}
}

void UPhosphorAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,Health,OldHealth);
}

void UPhosphorAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,Mana,OldMana);
}

void UPhosphorAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,Strength,OldStrength);
}

void UPhosphorAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,Intelligence,OldIntelligence);
}

void UPhosphorAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,Resilience,OldResilience);
}

void UPhosphorAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,Vigor,OldVigor);
}

void UPhosphorAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,Armor,OldArmor);
}

void UPhosphorAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,ArmorPenetration,OldArmorPenetration);
}

void UPhosphorAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,BlockChance,OldBlockChance);
}

void UPhosphorAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,CriticalHitChance,OldCriticalHitChance);
}

void UPhosphorAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,CriticalHitDamage,OldCriticalHitDamage);
}

void UPhosphorAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,CriticalHitResistance,OldCriticalHitResistance);
}

void UPhosphorAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,HealthRegeneration,OldHealthRegeneration);
}

void UPhosphorAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,ManaRegeneration,OldManaRegeneration);
}

void UPhosphorAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,MaxHealth,OldMaxHealth);
}

void UPhosphorAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,MaxMana,OldMaxMana);
}

void UPhosphorAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,FireResistance,OldFireResistance);
}

void UPhosphorAttributeSet::OnRep_LightingResistance(const FGameplayAttributeData& OldLightingResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,LightingResistance,OldLightingResistance);
}

void UPhosphorAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,ArcaneResistance,OldArcaneResistance);
}

void UPhosphorAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,PhysicalResistance,OldPhysicalResistance);
}
