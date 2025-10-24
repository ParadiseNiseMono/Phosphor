// copyright Paradise_NiseMono


#include "Actor/PhosphorProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Phosphor/Phosphor.h"

// Sets default values
APhosphorProjectile::APhosphorProjectile()
{
	PrimaryActorTick.bCanEverTick =false;

	bReplicates = true;
	SphereComponent=CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed=500.0f;
	ProjectileMovementComponent->MaxSpeed=500.0f;
	ProjectileMovementComponent->ProjectileGravityScale=0.0f;
}

void APhosphorProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&APhosphorProjectile::OnShpereOverlap);

	LoopingSoundComponent= UGameplayStatics::SpawnSoundAttached(ImpactSound,GetRootComponent());
	SetLifeSpan(LifeSpan);
}

void APhosphorProjectile::Destroyed()
{
	if (!bHit&&!HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
		LoopingSoundComponent->Stop();
	}
	Super::Destroyed();
}

void APhosphorProjectile::OnShpereOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	LoopingSoundComponent->Stop();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Other))
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		Destroy();
	}
	else
	{
		bHit=true;
	}
}


