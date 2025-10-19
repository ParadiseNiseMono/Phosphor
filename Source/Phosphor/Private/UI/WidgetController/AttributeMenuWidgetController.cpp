// copyright Paradise_NiseMono


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "PhosphorGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UPhosphorAttributeSet* AS=CastChecked<UPhosphorAttributeSet>(AttributeSet);

	check(AttributeInfo);
	FPhosphorAttributeInfo Info= AttributeInfo->FindAttributeInfoForTag(FPhosphorGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue= AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}
