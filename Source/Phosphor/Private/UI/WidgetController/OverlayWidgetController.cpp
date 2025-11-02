// copyright Paradise_NiseMono


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UPhosphorAttributeSet* PhosphorAttributeSet=CastChecked<UPhosphorAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(PhosphorAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PhosphorAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(PhosphorAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(PhosphorAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UPhosphorAttributeSet* PhosphorAttributeSet=CastChecked<UPhosphorAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PhosphorAttributeSet->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnHealthChanged.Broadcast(Data.NewValue);
				}
			);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PhosphorAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PhosphorAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PhosphorAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	if (UPhosphorAbilitySystemComponent* PhosphorAbilitySystemComponent=Cast<UPhosphorAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (PhosphorAbilitySystemComponent->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(PhosphorAbilitySystemComponent);
		}
		else
		{
			PhosphorAbilitySystemComponent->AbilitiesGivenDelegate.AddUObject(this,&UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		PhosphorAbilitySystemComponent->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag=FGameplayTag::RequestGameplayTag("Message");
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row= GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
				
			}
		}
	);
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UPhosphorAbilitySystemComponent* PhosphorAbilitySystemComponent)
{
	//TODO Get information about all given abilities,lookup their Ability Info and Broadcast it to Widget.
	if (!PhosphorAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this,PhosphorAbilitySystemComponent](const FGameplayAbilitySpec& Spec)
	{
		FPhosphorAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(PhosphorAbilitySystemComponent->GetAbilityTagFromSpec(Spec));
		Info.InputTag=PhosphorAbilitySystemComponent->GetInputTagFromSpec(Spec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	PhosphorAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

