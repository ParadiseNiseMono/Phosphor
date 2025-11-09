// copyright Paradise_NiseMono


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/PhosphorPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	OnPlayerSpellPointChangedDelegate.Broadcast(GetPhosphorPS()->GetPlayerSpellPoint());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetPhosphorASC()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FPhosphorAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	GetPhosphorPS()->OnSpellPointChangedDelegate.AddLambda([this](const int32 NewSpellPoint)
	{
		OnPlayerSpellPointChangedDelegate.Broadcast(NewSpellPoint);
	});
}
