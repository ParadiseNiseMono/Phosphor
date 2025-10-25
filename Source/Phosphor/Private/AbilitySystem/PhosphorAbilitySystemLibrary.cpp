// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"

#include "Game/PhosphorGameModeBase.h"
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

void UPhosphorAbilitySystemLibrary::InitializeDefaultAbilities(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor=ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo=CharacterClassInfo->GetClassDefaultInfo(CharacterClass);


	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level,PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,Level,SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level,VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UPhosphorAbilitySystemLibrary::GiveStartUpAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);

	for (TSubclassOf<UGameplayAbility> GameAbility:CharacterClassInfo->Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameAbility,1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UPhosphorAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	APhosphorGameModeBase* PhosphorGameModeBase= Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (PhosphorGameModeBase==nullptr) return nullptr;

	return PhosphorGameModeBase->CharacterClassInfo;
}
