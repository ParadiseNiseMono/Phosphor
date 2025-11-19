// copyright Paradise_NiseMono


#include "Actor/PhosphorFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueManager.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

void APhosphorFireBall::OnHit()
{

	if (GetOwner())
	{
		FGameplayCueParameters GameplayCueParameters;
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FPhosphorGameplayTags::Get().GameplayCue_FireBlast, GameplayCueParameters);
	}
	
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
		
	bHit=true;
}

void APhosphorFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeLine();
}

void APhosphorFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UPhosphorAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}
