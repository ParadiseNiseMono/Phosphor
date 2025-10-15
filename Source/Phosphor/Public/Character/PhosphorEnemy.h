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
protected:
	virtual void BeginPlay() override;
};
