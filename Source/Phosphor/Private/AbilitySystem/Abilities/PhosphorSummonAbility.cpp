// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

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
		const FVector ChosenLocation=Location+Direction*FMath::RandRange(MinSpawnDistance,MaxSpawnDistance);
		DrawDebugSphere(GetWorld(),ChosenLocation,18.f,12,FColor::Green,false,3.f);
		SpawnLocations.Add(ChosenLocation);
		UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		Location,
		Location+Direction*MaxSpawnDistance,
		4.f,
		FColor::Red,
		3.f);
	}

	return SpawnLocations;
}
