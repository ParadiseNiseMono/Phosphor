// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PhosphorHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UPhosphorUserWidget;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API APhosphorHUD : public AHUD
{
	GENERATED_BODY()


public:

	UPROPERTY()
	TObjectPtr<UPhosphorUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS);
protected:

private:
	UPROPERTY(EditAnywhere, Category = "WidgetController")
	TSubclassOf<UPhosphorUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "WidgetController")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
