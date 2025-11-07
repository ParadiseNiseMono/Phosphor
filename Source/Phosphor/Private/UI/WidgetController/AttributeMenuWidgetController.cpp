// copyright Paradise_NiseMono


#include "UI/WidgetController/AttributeMenuWidgetController.h"

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
	PhosphorPlayerState->OnSpellPointChangedDelegate.AddLambda([this](int32 NewSpellPoint)
	{
		OnPlayerSpellPointChangedDelegate.Broadcast(NewSpellPoint);
	});
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& GameplayAttribute) const
{
	check(AttributeInfo);
	FPhosphorAttributeInfo Info=AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue=GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
