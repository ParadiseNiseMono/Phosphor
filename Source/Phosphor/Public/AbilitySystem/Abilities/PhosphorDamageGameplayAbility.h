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

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromDefaults(AActor* TargetActor = nullptr, FVector InRadialDamageOrigin = FVector::ZeroVector) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffChance;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffDamage;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffFrequency;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffDuration;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DeathImpulseMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float RadialDamageOuterRadius = 0.f;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
