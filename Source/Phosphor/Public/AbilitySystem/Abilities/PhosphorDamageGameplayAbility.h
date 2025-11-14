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

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly,Category="Debuff")
	FScalableFloat DebuffChance;

	UPROPERTY(EditDefaultsOnly,Category="Debuff")
	FScalableFloat DebuffDamage;

	UPROPERTY(EditDefaultsOnly,Category="Debuff")
	FScalableFloat DebuffFrequency;

	UPROPERTY(EditDefaultsOnly,Category="Debuff")
	FScalableFloat DebuffDuration;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

};
