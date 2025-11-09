// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PhosphorWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

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

	UPROPERTY(BlueprintAssignable, Category="GAS|PlayerState")
	FOnPlayerStateChangedSignature OnPlayerSpellPointChangedDelegate;
};
