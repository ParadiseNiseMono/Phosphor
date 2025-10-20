// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/PhosphorWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
struct FPhosphorAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FPhosphorAttributeInfo&,Info);

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class PHOSPHOR_API UAttributeMenuWidgetController : public UPhosphorWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies()override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag,const FGameplayAttribute& GameplayAttribute) const;
};
