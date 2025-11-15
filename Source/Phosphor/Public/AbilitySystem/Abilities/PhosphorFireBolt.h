// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorProjectileSpell.h"
#include "PhosphorFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorFireBolt : public UPhosphorProjectileSpell
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BLueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FVector& SocketLocation, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:

	UPROPERTY(EditDefaultsOnly, Category= "FureBolt")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category= "FureBolt")
	int32 MaxNumProjectiles = 5;
};
