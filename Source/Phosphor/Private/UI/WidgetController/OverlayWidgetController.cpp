// copyright Paradise_NiseMono


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/PhosphorPlayerState.h"

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
	APhosphorPlayerState* PhosphorPlayerState=CastChecked<APhosphorPlayerState>(PlayerState);
	
	PhosphorPlayerState->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);

	PhosphorPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});

	const UPhosphorAttributeSet* PhosphorAttributeSet=CastChecked<UPhosphorAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PhosphorAttributeSet->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnHealthChanged.Broadcast(Data.NewValue);
				}
			);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PhosphorAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PhosphorAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	PhosphorAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);
	
	if (UPhosphorAbilitySystemComponent* PhosphorAbilitySystemComponent=Cast<UPhosphorAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (PhosphorAbilitySystemComponent->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(PhosphorAbilitySystemComponent);
		}
		else
		{
			PhosphorAbilitySystemComponent->AbilitiesGivenDelegate.AddUObject(this,&UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		PhosphorAbilitySystemComponent->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UPhosphorAbilitySystemComponent* PhosphorAbilitySystemComponent)
{
	//TODO Get information about all given abilities,lookup their Ability Info and Broadcast it to Widget.
	if (!PhosphorAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this,PhosphorAbilitySystemComponent](const FGameplayAbilitySpec& Spec)
	{
		FPhosphorAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(PhosphorAbilitySystemComponent->GetAbilityTagFromSpec(Spec));
		Info.InputTag=PhosphorAbilitySystemComponent->GetInputTagFromSpec(Spec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	PhosphorAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXP) const
{
	const APhosphorPlayerState* PhosphorPlayerState=CastChecked<APhosphorPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo=PhosphorPlayerState->LevelUpInfo;

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

