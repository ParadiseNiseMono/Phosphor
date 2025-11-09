// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PhosphorGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	FGameplayTag StartUpTag;

	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static  FString GetLockedDescription(int32 Level);
	
protected:

	float GetManaCost(const int32 Level = 1) const;
	float GetCooldown(const int32 Level = 1) const;
};

