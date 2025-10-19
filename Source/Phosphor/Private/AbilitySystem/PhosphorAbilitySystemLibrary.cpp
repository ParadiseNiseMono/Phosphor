// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/PhosphorPlayerState.h"
#include "UI/HUD/PhosphorHUD.h"
#include "UI/WidgetController/PhosphorWidgetController.h"

UOverlayWidgetController* UPhosphorAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APhosphorHUD* PhosphorHUD=Cast<APhosphorHUD>(PC->GetHUD()))
		{
			APhosphorPlayerState* PS=PC->GetPlayerState<APhosphorPlayerState>();
			UAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
			UAttributeSet* AS=PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return  PhosphorHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UPhosphorAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APhosphorHUD* PhosphorHUD=Cast<APhosphorHUD>(PC->GetHUD()))
		{
			APhosphorPlayerState* PS=PC->GetPlayerState<APhosphorPlayerState>();
			UAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
			UAttributeSet* AS=PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return  PhosphorHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}
