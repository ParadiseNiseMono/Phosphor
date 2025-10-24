// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorProjectileSpell.h"

#include "Actor/PhosphorProjectile.h"
#include "Interaction/CombatInterface.h"


void UPhosphorProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UPhosphorProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer=GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;

	ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation=(ProjectileTargetLocation-SocketLocation).Rotation();
		Rotation.Pitch=0.0f;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		//TODO:Set the Projectile Rotation
		
		APhosphorProjectile* PhosphorProjectile=GetWorld()->SpawnActorDeferred<APhosphorProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//TODO:Give the Projectile a GameplayEffectSpec to causing damage
		
		PhosphorProjectile->FinishSpawning(SpawnTransform);
	}
}
