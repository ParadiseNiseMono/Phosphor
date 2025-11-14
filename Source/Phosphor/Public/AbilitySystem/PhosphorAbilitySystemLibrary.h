// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "PhosphorAbilitySystemLibrary.generated.h"

struct FDamageEffectParams;
class UAbilityInfo;
class APhosphorHUD;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
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
	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|WidgetController" , meta=(DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject,FWidgetControllerParams& OutWCParams,APhosphorHUD*& OutPhosphorHUD);
	
	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAbilities(const UObject* WorldContextObject,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartUpAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockHit(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bInIsBlockHit);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject,TArray<AActor*>& OutOverlappingActors,const TArray<AActor*>& ActorsToIgnore,float Radius,const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static bool IsNotFriend(AActor* FirstActor,AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);


	UFUNCTION(BlueprintCallable, Category = "PhosphorAbilitySystemLibrary|GameplayEffects")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass,int32 CharacterLevel);
};


