// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PhosphorWidgetController.h"
#include "GameplayTagContainer.h"
#include "PhosphorGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSingnature, bool , bSpendPointsButtonEnabled , bool , bEquipButtonEnabled);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PHOSPHOR_API USpellMenuWidgetController : public UPhosphorWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChangedSignature OnPlayerSpellPointChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSingnature SpellGlobeSelectedDelegate;

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointsButtonPressed();

private:

	static void ShouldEnableButtons(const FGameplayTag& AbilityStatusTag , const int32 SpellPoints , bool& bShouldEnableSpendPointsButton , bool& bShouldEnableEquipButton);

	FSelectedAbility SelectedAbility = FSelectedAbility(FPhosphorGameplayTags::Get().Abilities_None , FPhosphorGameplayTags::Get().Abilities_Status_Locked);
	int32 CurrentSpellPoint = 0;
};
