// copyright Paradise_NiseMono


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/PhosphorAttributeSet.h"

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
		PhosphorAttributeSet->GetHealthAttribute()).AddUObject(this,&UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PhosphorAttributeSet->GetMaxHealthAttribute()).AddUObject(this,&UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PhosphorAttributeSet->GetManaAttribute()).AddUObject(this,&UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PhosphorAttributeSet->GetMaxManaAttribute()).AddUObject(this,&UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged (const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
