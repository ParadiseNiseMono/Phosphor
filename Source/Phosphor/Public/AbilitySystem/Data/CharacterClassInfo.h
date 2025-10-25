// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elemantalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Category="Class Default")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Class Default")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;	
	
	UPROPERTY(EditDefaultsOnly,Category="Common Class Default")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly,Category="Common Class Default")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly,Category="Common Class Default")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly,Category="Common Class Default|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficient;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
