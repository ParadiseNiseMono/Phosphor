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
}

UAbilitySystemComponent* APhosphorPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APhosphorPlayerState::OnRep_Level(int32 OldLevel)
{
	
}
