// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAttributeSet.h"
#include "Net/UnrealNetwork.h"

UPhosphorAttributeSet::UPhosphorAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitMana(100.0f);
	InitMaxMana(100.0f);
}

void UPhosphorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPhosphorAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
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
