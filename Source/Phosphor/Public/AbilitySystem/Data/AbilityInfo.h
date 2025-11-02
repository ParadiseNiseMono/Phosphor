// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FPhosphorAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityTag=FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<const UMaterialInstance> BackgroundMaterial=nullptr;
};

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Ability Info")
	TArray<FPhosphorAbilityInfo> AbilityInformation;

	FPhosphorAbilityInfo FindAbilityByTag(const FGameplayTag& AbilityTag,bool bLogNotFound=false) const;
};
