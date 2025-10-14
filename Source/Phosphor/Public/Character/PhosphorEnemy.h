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
	virtual void HightLightActor() override;
	virtual void UnHightLightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHightLighted = false;
};
