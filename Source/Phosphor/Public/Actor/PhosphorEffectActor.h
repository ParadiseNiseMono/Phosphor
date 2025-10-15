// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhosphorEffectActor.generated.h"

class USphereComponent;

UCLASS()
class PHOSPHOR_API APhosphorEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APhosphorEffectActor();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnOverLap(UPrimitiveComponent* OverLappedComponent,AActor* OtherActor
		,UPrimitiveComponent* OtherComponent,int32 OtherBodyIndex,bool FromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
};
