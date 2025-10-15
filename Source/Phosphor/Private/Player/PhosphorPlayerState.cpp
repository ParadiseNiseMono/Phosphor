// copyright Paradise_NiseMono


#include "Player/PhosphorPlayerState.h"

#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAttributeSet.h"

APhosphorPlayerState::APhosphorPlayerState()
{
	AbilitySystemComponent=CreateDefaultSubobject<UPhosphorAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet=CreateDefaultSubobject<UPhosphorAttributeSet>("AttributeSet");
	SetNetUpdateFrequency(100.0f);
}

UAbilitySystemComponent* APhosphorPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
