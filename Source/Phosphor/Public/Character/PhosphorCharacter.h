// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Character/PhosphorCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "PhosphorCharacter.generated.h"

class APhosphorPlayerState;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API APhosphorCharacter : public APhosphorCharacterBase,public IPlayerInterface
{
	GENERATED_BODY()

public:
	APhosphorCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/*Player Interface*/
	virtual void AddToXP_Implementation(int32 InXP) override;
	/*End Player Interface*/
	
	/*Combat Interface*/
	virtual int32 GetPlayerLevel() override;
	/*End Combat Interface*/
private:
	virtual  void InitAbilityActorInfo() override;
};
