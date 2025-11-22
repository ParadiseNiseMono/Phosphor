// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PhosphorWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FPhosphorAbilityInfo;
class UPhosphorAbilitySystemComponent;
class UAbilityInfo;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MessageTag=FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message=FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UPhosphorUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image=nullptr;
};

class UPhosphorUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChangedSignature, int32, NewLevel, bool, bIsLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FUIWidgetRow,Row);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PHOSPHOR_API UOverlayWidgetController : public UPhosphorWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Message")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnLevelChangedSignature OnPlayerLevelChangedDelegate;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const;
	
	template<typename  T>
	T* GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& Tag);

	void OnXPChanged(const int32 NewXP);
};
template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
