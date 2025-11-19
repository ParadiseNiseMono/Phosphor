// copyright Paradise_NiseMono


#include "Actor/PhosphorFireBall.h"

void APhosphorFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void APhosphorFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
