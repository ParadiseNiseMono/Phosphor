// copyright Paradise_NiseMono


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/PhosphorPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	for (auto& Pair:GetPhosphorAS()->TagsToAttributes )
	{
		BroadcastAttributeInfo(Pair.Key,Pair.Value());
	}
	OnPlayerAttributePointChangedDelegate.Broadcast(GetPhosphorPS()->GetPlayerAttributePoint());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	for (auto& Pair:GetPhosphorAS()->TagsToAttributes )
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key,Pair.Value());
			}
			);
	}
	GetPhosphorPS()->OnAttributePointChangedDelegate.AddLambda([this](int32 NewAttributePoint)
	{
		OnPlayerAttributePointChangedDelegate.Broadcast(NewAttributePoint);
	});
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetPhosphorASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& GameplayAttribute) const
{
	check(AttributeInfo);
	FPhosphorAttributeInfo Info=AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue=GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
