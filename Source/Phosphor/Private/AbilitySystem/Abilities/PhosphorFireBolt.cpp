// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorFireBolt.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "Actor/PhosphorProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"

FString UPhosphorFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage=Damage.GetValueAtLevel(Level);
	
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float ScaledDebuffChance = DebuffChance.GetValueAtLevel(Level);
	const float ScaledDebuffDamage = DebuffDamage.GetValueAtLevel(Level);
	const float ScaledDebuffDuration = DebuffDuration.GetValueAtLevel(Level);
	const float ScaledDebuffFrequency = DebuffFrequency.GetValueAtLevel(Level);
	if (Level==1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Shoot a fireball at the enemy's face</>\n\n"
			"<Default>Damage per fireball:</><Damage>%d</>\n\n"
			"<Default>There is</><Damage>%.1f</><Default>chance of causing a burning effect.</>\n\n"
			"<Default>After burning, it deals</><Damage>%.1f</><Default>fire damage per second.</>\n\n"
			"<Default>Each time it deals</><Damage>%.1f</><Default>of damage.</>\n\n"
			"<Default>Duration</><Damage>%.1f</><Default>seconds</>\n\n"
			)
			
			//Values
			, Level, ManaCost, Cooldown, ScaledDamage, ScaledDebuffChance, ScaledDebuffFrequency, ScaledDebuffDamage, ScaledDebuffDuration);
	}
	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Shoot %d fire a fireball at the enemy's face.</>\n\n"
			"<Default>Damage per fireball:</><Damage>%d</>\n\n"
			"<Default>There is</><Damage>%.1f</><Default>chance of causing a burning effect.</>\n\n"
			"<Default>After burning, it deals</><Damage>%.1f</><Default>fire damage per second.</>\n\n"
			"<Default>Each time it deals</><Damage>%.1f</><Default>of damage.</>\n\n"
			"<Default>Duration</><Damage>%.1f</><Default>seconds</>\n\n"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, NumProjectiles), ScaledDamage, ScaledDebuffChance, ScaledDebuffFrequency, ScaledDebuffDamage, ScaledDebuffDuration);
	}
}

FString UPhosphorFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage=Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float ScaledDebuffChance = DebuffChance.GetValueAtLevel(Level);
	const float ScaledDebuffDamage = DebuffDamage.GetValueAtLevel(Level);
	const float ScaledDebuffDuration = DebuffDuration.GetValueAtLevel(Level);
	const float ScaledDebuffFrequency = DebuffFrequency.GetValueAtLevel(Level);
	return FString::Printf(TEXT(
			//Title
			"<Title>NEXT LEVEL</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Shoot %d fire a fireball at the enemy's face.</>\n\n"
			"<Default>Damage per fireball:</><Damage>%d</>\n\n"
			"<Default>There is</><Damage>%.1f</><Default>chance of causing a burning effect.</>\n\n"
			"<Default>After burning, it deals</><Damage>%.1f</><Default>fire damage per second.</>\n\n"
			"<Default>Each time it deals</><Damage>%.1f</><Default>of damage.</>\n\n"
			"<Default>Duration</><Damage>%.1f</><Default>seconds</>\n\n"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, NumProjectiles), ScaledDamage, ScaledDebuffChance, ScaledDebuffFrequency, ScaledDebuffDamage, ScaledDebuffDuration);
}

void UPhosphorFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FVector& SocketLocation,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer=GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;
	
	FRotator Rotation=(ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch=PitchOverride;

	const FVector Forward = Rotation.Vector();

	const int32 EffectiveNumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());

	TArray<FRotator> Rotations = UPhosphorAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		APhosphorProjectile* Projectile=GetWorld()->SpawnActorDeferred<APhosphorProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromDefaults();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovementComponent->bIsHomingProjectile = bLaunchHomingTarget;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
