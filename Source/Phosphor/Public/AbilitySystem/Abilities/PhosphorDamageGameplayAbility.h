// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorGameplayAbility.h"
#include "PhosphorDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorDamageGameplayAbility : public UPhosphorGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* Target);
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,Category="Combat")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;
};
