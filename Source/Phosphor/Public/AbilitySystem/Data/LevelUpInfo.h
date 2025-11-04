// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FPhosphorLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 LevelUpRequirement=0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 AttributePointReward=1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 SpellPointReward=1;
};

/**
 * 
 */
UCLASS()
class PHOSPHOR_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="LevelUpInfo")
	TArray<FPhosphorLevelUpInfo> LevelUpInformation;
	
	int32 FindLevelForXP(int32 XP) const;
};
