// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"


UPhosphorAttributeSet::UPhosphorAttributeSet()
{
	InitHealth(50.0f);
	InitMaxHealth(100.0f);
	InitMana(10.0f);
	InitMaxMana(50.0f);
}

void UPhosphorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
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
}

void UPhosphorAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,Health,OldHealth);
}

void UPhosphorAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,MaxHealth,OldMaxHealth);
}

void UPhosphorAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,Mana,OldMana);
}

void UPhosphorAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPhosphorAttributeSet,MaxMana,OldMaxMana);
}
