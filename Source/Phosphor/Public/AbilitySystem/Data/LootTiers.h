// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawing")
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawing")
	float ChanceToSpawn = 0.f;

	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawing")
	int32 MaxNumberToSpawn = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawing")
	bool bLootLevelOverride = true;
};

/**
 * 
 */
UCLASS()
class PHOSPHOR_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();

	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawing")
	TArray<FLootItem> LootItems;
};
