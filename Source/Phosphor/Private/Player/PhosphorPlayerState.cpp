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

void APhosphorPlayerState::AddToXP(const int32 AddXP)
{
	XP+=AddXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void APhosphorPlayerState::AddLevel(const int32 AddLevel)
{
	Level+=AddLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}
