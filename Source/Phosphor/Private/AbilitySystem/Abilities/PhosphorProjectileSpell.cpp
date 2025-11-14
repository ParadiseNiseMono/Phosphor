// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/PhosphorProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Phosphor/Public/PhosphorGameplayTags.h"


void UPhosphorProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UPhosphorProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation,const FVector& SocketLocation,bool bOverridePitch,float PitchOverride)
{
	const bool bIsServer=GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;
	
	FRotator Rotation=(ProjectileTargetLocation-SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch=PitchOverride;
	}
	//Rotation.Pitch=0.0f;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	APhosphorProjectile* PhosphorProjectile=GetWorld()->SpawnActorDeferred<APhosphorProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	PhosphorProjectile->DamageEffectParams = MakeDamageEffectParamsFromDefaults();
		
	PhosphorProjectile->FinishSpawning(SpawnTransform);
}

