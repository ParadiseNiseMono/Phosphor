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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FScalableFloat Damage;
};
