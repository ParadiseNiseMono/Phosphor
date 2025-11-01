// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorGameplayAbility.h"
#include "PhosphorSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorSummonAbility : public UPhosphorGameplayAbility
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure,Category="Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	int32 NumMinions=5;

	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MinSpawnDistance=150.f;

	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MaxSpawnDistance=400.f;

	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float SpawnSpread=90.f;

};
