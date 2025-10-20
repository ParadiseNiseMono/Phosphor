// copyright Paradise_NiseMono


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "PhosphorGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UPhosphorAttributeSet* AS=CastChecked<UPhosphorAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair:AS->TagsToAttributes )
	{
		FPhosphorAttributeInfo Info=AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue=Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}
