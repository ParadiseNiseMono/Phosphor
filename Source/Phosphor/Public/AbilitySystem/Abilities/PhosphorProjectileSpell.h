// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorGameplayAbility.h"
#include "PhosphorProjectileSpell.generated.h"

class APhosphorProjectile;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorProjectileSpell : public UPhosphorGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<APhosphorProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable,Category="Abilities|Phosphor|Projectile Spell")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
};
