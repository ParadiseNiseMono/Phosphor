// copyright Paradise_NiseMono


#include "UI/WidgetController/PhosphorWidgetController.h"

#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/PhosphorPlayerController.h"
#include "Player/PhosphorPlayerState.h"

void UPhosphorWidgetController::SetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController=WCParams.PlayerController;
	PlayerState=WCParams.PlayerState;
	AbilitySystemComponent=WCParams.AbilitySystemComponent;
	AttributeSet=WCParams.AttributeSet;
}

void UPhosphorWidgetController::BroadcastInitialValues()
{
	
}

void UPhosphorWidgetController::BindCallbacksToDependencies()
{
	
}

void UPhosphorWidgetController::BroadcastAbilityInfo()
{
	if (!GetPhosphorASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& Spec)
	{
		FPhosphorAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(PhosphorAbilitySystemComponent->GetAbilityTagFromSpec(Spec));
		Info.InputTag=PhosphorAbilitySystemComponent->GetInputTagFromSpec(Spec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetPhosphorASC()->ForEachAbility(BroadcastDelegate);
}

APhosphorPlayerController* UPhosphorWidgetController::GetPhosphorPC()
{
	if (PhosphorPlayerController == nullptr)
	{
		PhosphorPlayerController = Cast<APhosphorPlayerController>(PlayerController);
	}
	return PhosphorPlayerController;
}

APhosphorPlayerState* UPhosphorWidgetController::GetPhosphorPS()
{
	if (PhosphorPlayerState == nullptr)
	{
		PhosphorPlayerState = Cast<APhosphorPlayerState>(PlayerState);
	}
	return PhosphorPlayerState;
}

UPhosphorAbilitySystemComponent* UPhosphorWidgetController::GetPhosphorASC()
{
	if (PhosphorAbilitySystemComponent == nullptr)
	{
		PhosphorAbilitySystemComponent = Cast<UPhosphorAbilitySystemComponent>(AbilitySystemComponent);
	}
	return PhosphorAbilitySystemComponent;
}

UPhosphorAttributeSet* UPhosphorWidgetController::GetPhosphorAS()
{
	if (PhosphorAttributeSet == nullptr)
	{
		PhosphorAttributeSet=Cast<UPhosphorAttributeSet>(AttributeSet);
	}
	return PhosphorAttributeSet;
}
