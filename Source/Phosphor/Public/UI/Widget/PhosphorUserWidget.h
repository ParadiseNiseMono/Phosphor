// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PhosphorUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
