// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "PhosphorAbilityTypes.h"
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

	FDamageEffectParams MakeDamageEffectParamsFromDefaults(AActor* TargetActor = nullptr) const;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FScalableFloat DebuffChance;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FScalableFloat DebuffDamage;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FScalableFloat DebuffFrequency;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FScalableFloat DebuffDuration;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DeathImpulseMagnitude = 60.f;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

};
