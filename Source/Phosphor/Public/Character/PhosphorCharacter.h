// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Character/PhosphorCharacterBase.h"
#include "PhosphorCharacter.generated.h"

class APhosphorPlayerState;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API APhosphorCharacter : public APhosphorCharacterBase
{
	GENERATED_BODY()

public:
	APhosphorCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
private:
	virtual  void InitAbilityActorInfo() override;
};
