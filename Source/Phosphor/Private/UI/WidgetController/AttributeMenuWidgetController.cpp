// copyright Paradise_NiseMono


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/PhosphorPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UPhosphorAttributeSet* AS=CastChecked<UPhosphorAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair:AS->TagsToAttributes )
	{
		BroadcastAttributeInfo(Pair.Key,Pair.Value());
	}
	APhosphorPlayerState* PhosphorPlayerState=CastChecked<APhosphorPlayerState>(PlayerState);
	OnPlayerAttributePointChangedDelegate.Broadcast(PhosphorPlayerState->GetPlayerAttributePoint());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UPhosphorAttributeSet* AS=CastChecked<UPhosphorAttributeSet>(AttributeSet);

	check(AttributeInfo);
	for (auto& Pair:AS->TagsToAttributes )
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key,Pair.Value());
			}
			);
	}

	APhosphorPlayerState* PhosphorPlayerState=CastChecked<APhosphorPlayerState>(PlayerState);
	PhosphorPlayerState->OnAttributePointChangedDelegate.AddLambda([this](int32 NewAttributePoint)
	{
		OnPlayerAttributePointChangedDelegate.Broadcast(NewAttributePoint);
	});
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UPhosphorAbilitySystemComponent* PhosphorAbilitySystemComponent=CastChecked<UPhosphorAbilitySystemComponent>(AbilitySystemComponent);
	PhosphorAbilitySystemComponent->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& GameplayAttribute) const
{
	check(AttributeInfo);
	FPhosphorAttributeInfo Info=AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue=GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
