// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorDamageGameplayAbility.h"
#include "PhosphorFireBlast.generated.h"

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
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FireBlast")
	int32 NumFireBalls = 12;
	
};
