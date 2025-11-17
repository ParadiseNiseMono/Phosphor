// copyright Paradise_NiseMono


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UPhosphorAbilitySystemComponent* PhosphorASC= Cast<UPhosphorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		PhosphorASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (UPhosphorAbilitySystemComponent* PhosphorASC= Cast<UPhosphorAbilitySystemComponent>(ASC))
			{
				PhosphorASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}
