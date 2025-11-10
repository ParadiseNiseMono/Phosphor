// copyright Paradise_NiseMono


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "PhosphorGameplayTags.h"
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
	GetPhosphorASC()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 Level)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bShouldSpendPoint = false;
			bool bShouldEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoint , bShouldSpendPoint, bShouldEquip);
			FString Description;
			FString NextLevelDescription;
			GetPhosphorASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bShouldSpendPoint, bShouldEquip, Description, NextLevelDescription);
		}
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
		CurrentSpellPoint = NewSpellPoint;
		
		bool bShouldSpendPoint = false;
		bool bShouldEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoint , bShouldSpendPoint, bShouldEquip);
		FString Description;
		FString NextLevelDescription;
		GetPhosphorASC()->GetDescriptionByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bShouldSpendPoint, bShouldEquip, Description, NextLevelDescription);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FPhosphorGameplayTags GameplayTags = FPhosphorGameplayTags::Get();
	const int32 SpellPoints = GetPhosphorPS()->GetPlayerSpellPoint();
	FGameplayTag AbilityStatusTag;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetPhosphorASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatusTag = GameplayTags.Abilities_Status_Locked;	
	}
	else
	{
		AbilityStatusTag = GetPhosphorASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatusTag;

	bool bShouldSpendPoint = false;
	bool bShouldEquip = false;
	ShouldEnableButtons(AbilityStatusTag, SpellPoints, bShouldSpendPoint, bShouldEquip);
	FString Description;
	FString NextLevelDescription;
	GetPhosphorASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bShouldSpendPoint, bShouldEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointsButtonPressed()
{
	GetPhosphorASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
}

void USpellMenuWidgetController::GlobeDeselect()
{
	SelectedAbility.Ability = FPhosphorGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FPhosphorGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatusTag, const int32 SpellPoints,
                                                     bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton)
{
	const FPhosphorGameplayTags GameplayTags = FPhosphorGameplayTags::Get();

	bShouldEnableSpendPointsButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;
		}
	}
	else if (AbilityStatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;
		}
	}
	else if (AbilityStatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;
		}
	}
}
