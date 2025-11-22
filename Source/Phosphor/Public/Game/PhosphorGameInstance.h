// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PhosphorGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
