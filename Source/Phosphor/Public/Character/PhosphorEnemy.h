// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Character/PhosphorCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "PhosphorEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class APhosphorAIController;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API APhosphorEnemy : public APhosphorCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:
	APhosphorEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	/*EnemyInterface*/
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	/*end EnemyInterface*/

	/*Combat Interface*/
	FORCEINLINE virtual int32 GetPlayerLevel() override {return Level;};
	virtual void Die() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	/*End Combat Interface*/

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallBackTag,int32 NewCount);

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReacting=false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed=300.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float LifeSpan=5.0f;

	UPROPERTY(BlueprintReadWrite,Category="Combat")
	TObjectPtr<AActor> CombatTarget;
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttribute() const override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharacterDefaultClass")
	int32 Level=1;



	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="CharacterDefaultClass")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<APhosphorAIController> PhosphorAIController;
	
};
