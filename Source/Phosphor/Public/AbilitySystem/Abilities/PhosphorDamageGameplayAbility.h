// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorGameplayAbility.h"
#include "PhosphorDamageGameplayAbility.generated.h"

struct FTaggedMontage;
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

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	float GetDamageByDamageType(int32 InLevel, const FGameplayTag& DamageType);
};
