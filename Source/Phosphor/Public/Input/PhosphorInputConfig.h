// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PhosphorInputConfig.generated.h"

USTRUCT()
struct FPhosphorInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction=nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag=FGameplayTag();
};

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorInputConfig : public UDataAsset
{

	GENERATED_BODY()

public:
	const UInputAction* FindAbilityInputForTag(const FGameplayTag& InputTag,bool bLogNotFound=false) const;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FPhosphorInputAction> InputActions;
};
