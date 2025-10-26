// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "PhosphorAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAbilities(const UObject* WorldContextObject,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartUpAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockHit(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bInIsBlockHit);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bInIsCriticalHit);
};
