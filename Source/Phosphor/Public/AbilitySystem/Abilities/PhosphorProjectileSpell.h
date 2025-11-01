// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorDamageGameplayAbility.h"
#include "PhosphorProjectileSpell.generated.h"

class APhosphorProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorProjectileSpell : public UPhosphorDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<APhosphorProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable,Category="Abilities|Projectile Spell")
	void SpawnProjectile(const FVector& ProjectileTargetLocation,const FVector& SocketLocation,bool bOverridePitch=false,float PitchOverride=0.f);
	
};
