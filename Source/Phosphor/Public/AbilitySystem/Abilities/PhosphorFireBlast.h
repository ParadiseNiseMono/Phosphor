// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorDamageGameplayAbility.h"
#include "PhosphorFireBlast.generated.h"

class APhosphorFireBall;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorFireBlast : public UPhosphorDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<APhosphorFireBall*> SpawnFireBalls();
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FireBlast")
	int32 NumFireBalls = 12;

private:

	UPROPERTY(EditDefaultsOnly, Category="FireBlast")
	TSubclassOf<APhosphorFireBall> FireBallClass;
	
};
