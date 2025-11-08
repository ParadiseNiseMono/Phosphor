// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PhosphorWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API USpellMenuWidgetController : public UPhosphorWidgetController
{
	GENERATED_BODY()

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
};
