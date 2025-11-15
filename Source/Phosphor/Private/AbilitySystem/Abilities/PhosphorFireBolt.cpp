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
			"<Default>Shot a Fire Blot</>\n\n"
			"<Default>Each bolt can cause </><Damage>%d</><Default> damage</>\n\n"
			"<Default>Fire Damage with a chance to burn</>"
			)
			
			//Values
			, Level, ManaCost, Cooldown, ScaledDamage);
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
			"<Default>Shot %d Fire Blots</>\n\n"
			"<Default>Each bolt can cause </><Damage>%d</><Default> damage</>\n\n"
			"<Default>Fire Damage with a chance to burn</>"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, NumProjectiles), ScaledDamage);
	}
}

FString UPhosphorFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage=Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			//Title
			"<Title>NEXT LEVEL</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Shot %d Fire Blots</>\n\n"
			"<Default>Each bolt can cause </><Damage>%d</><Default> damage</>\n\n"
			"<Default>Fire Damage with a chance to burn</>"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, NumProjectiles), ScaledDamage);
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
