// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhosphorProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class PHOSPHOR_API APhosphorProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APhosphorProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnShpereOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

};
