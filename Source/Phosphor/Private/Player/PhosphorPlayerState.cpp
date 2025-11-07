// copyright Paradise_NiseMono


#include "Player/PhosphorPlayerState.h"

#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "Net/UnrealNetwork.h"

APhosphorPlayerState::APhosphorPlayerState()
{
	AbilitySystemComponent=CreateDefaultSubobject<UPhosphorAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet=CreateDefaultSubobject<UPhosphorAttributeSet>("AttributeSet");
	SetNetUpdateFrequency(100.0f);
}

void APhosphorPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APhosphorPlayerState,Level);
	DOREPLIFETIME(APhosphorPlayerState,XP);
	DOREPLIFETIME(APhosphorPlayerState,AttributePoint);
	DOREPLIFETIME(APhosphorPlayerState,SpellPoint);
}

UAbilitySystemComponent* APhosphorPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APhosphorPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void APhosphorPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void APhosphorPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
	OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void APhosphorPlayerState::OnRep_SpellPoint(int32 OldSpellPoint)
{
	OnSpellPointChangedDelegate.Broadcast(SpellPoint);
}


void APhosphorPlayerState::SetXP(const int32 NewXP)
{
	XP=NewXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void APhosphorPlayerState::SetLevel(const int32 NewLevel)
{
	Level=NewLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void APhosphorPlayerState::SetAttributePoint(const int32 NewAttributePoint)
{
	AttributePoint=NewAttributePoint;
	OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void APhosphorPlayerState::SetSpellPoint(const int32 NewSpellPoint)
{
	SpellPoint=NewSpellPoint;
	OnSpellPointChangedDelegate.Broadcast(SpellPoint);
}

void APhosphorPlayerState::AddToXP(const int32 AddXP)
{
	XP+=AddXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void APhosphorPlayerState::AddToLevel(const int32 AddLevel)
{
	Level+=AddLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void APhosphorPlayerState::AddToAttributePoint(const int32 AddAttributePoint)
{
	AttributePoint+=AddAttributePoint;
	OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void APhosphorPlayerState::AddToSpellPoint(const int32 AddSpellPoint)
{
	SpellPoint+=AddSpellPoint;
	OnSpellPointChangedDelegate.Broadcast(SpellPoint);
}
