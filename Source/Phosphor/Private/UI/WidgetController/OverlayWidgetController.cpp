// copyright Paradise_NiseMono


#include "UI/WidgetController/OverlayWidgetController.h"

#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/PhosphorPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetPhosphorAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetPhosphorAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetPhosphorAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetPhosphorAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	
	GetPhosphorPS()->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);

	GetPhosphorPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});
	

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetPhosphorAS()->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnHealthChanged.Broadcast(Data.NewValue);
				}
			);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetPhosphorAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetPhosphorAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetPhosphorAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);
	
	if (GetPhosphorASC())
	{
		GetPhosphorASC()->AbilityEquipped.AddUObject(this,&UOverlayWidgetController::OnAbilityEquipped);
		if (GetPhosphorASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetPhosphorASC()->AbilitiesGivenDelegate.AddUObject(this,&UOverlayWidgetController::BroadcastAbilityInfo);
		}
		GetPhosphorASC()->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FPhosphorGameplayTags GameplayTags=FPhosphorGameplayTags::Get();

	FPhosphorAbilityInfo LastSlotInfo;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	//Broadcast empty info if PreviousSlot is a valid slot.only if equipping an already equipped ability.
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FPhosphorAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.InputTag = Slot;
	Info.StatusTag = Status;
	AbilityInfoDelegate.Broadcast(Info);
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo=GetPhosphorPS()->LevelUpInfo;

	checkf(LevelUpInfo,TEXT("LevelUpInfo is null"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel=LevelUpInfo->LevelUpInformation.Num();

	if (Level<=MaxLevel&&Level>0)
	{
		const int32 LevelUpRequirement=LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement=LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		const int32 DeltaLevelUpRequirement=LevelUpRequirement-PreviousLevelUpRequirement;
		const int32 XPForThisLevel=NewXP-PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel)/static_cast<float>(DeltaLevelUpRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

