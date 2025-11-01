// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorSummonAbility.h"

TArray<FVector> UPhosphorSummonAbility::GetSpawnLocations()
{
	const FVector Forward=GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location=GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread=SpawnSpread/NumMinions;

	const FVector LeftOfSpread=Forward.RotateAngleAxis(-SpawnSpread/2.f,FVector::UpVector);
	
	const FVector RightOfSpread=Forward.RotateAngleAxis(SpawnSpread/2.f,FVector::UpVector);

	TArray<FVector> SpawnLocations;
	for (int i = 0; i < NumMinions; i++)
	{
		const FVector Direction=LeftOfSpread.RotateAngleAxis(i*DeltaSpread,FVector::UpVector);
		FVector ChosenLocation=Location+Direction*FMath::RandRange(MinSpawnDistance,MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(
			Hit,ChosenLocation+FVector(0.f,0.f,400.f),ChosenLocation-FVector(0.f,0.f,400.f),ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenLocation=Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenLocation);
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UPhosphorSummonAbility::GetRandomMinionClass()
{
	const int32 Selection=FMath::RandRange(0,MinionClasses.Num()-1);
	return MinionClasses[Selection];
}
