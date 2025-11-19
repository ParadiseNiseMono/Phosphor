// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Actor/PhosphorProjectile.h"
#include "PhosphorFireBall.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API APhosphorFireBall : public APhosphorProjectile
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeLine();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
protected:
	virtual void BeginPlay() override;
	
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
};
