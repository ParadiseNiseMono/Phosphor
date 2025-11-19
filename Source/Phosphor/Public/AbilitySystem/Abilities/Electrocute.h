// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UElectrocute : public UPhosphorBeamSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
