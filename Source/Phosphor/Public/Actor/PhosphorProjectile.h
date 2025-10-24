// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "PhosphorProjectile.generated.h"

struct FGameplayEffectSpecHandle;
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;
UCLASS()
class PHOSPHOR_API APhosphorProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APhosphorProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	// meta = (ExposeOnSpawn = "true") 讓這個變數可以在藍圖的 SpawnActor 節點上顯示為輸入引腳
	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn=true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	void OnShpereOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	bool bHit=false;

	UPROPERTY(EditAnywhere)
	float LifeSpan=10.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
};
