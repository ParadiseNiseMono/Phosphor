// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Character/PhosphorCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "PhosphorEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API APhosphorEnemy : public APhosphorCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:
	APhosphorEnemy();
	
	/*EnemyInterface*/
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	/*end EnemyInterface*/

	/*Combat Interface*/
	FORCEINLINE virtual int32 GetPlayerLevel() override {return Level;};
	/*End Combat Interface*/
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharacterDefaultClass")
	int32 Level=1;
	

};
