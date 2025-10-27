// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PhosphorAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class PHOSPHOR_API APhosphorAIController : public AAIController
{
	GENERATED_BODY()

public:

	APhosphorAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
