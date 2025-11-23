// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Character/PhosphorCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "PhosphorEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class APhosphorAIController;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API APhosphorEnemy : public APhosphorCharacterBase,public IEnemyInterface,public IHighlightInterface
{
	GENERATED_BODY()

public:
	APhosphorEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	/*HighlightInterface*/
	virtual void HighLightActor_Implementation() override;
	virtual void UnHighLightActor_Implementation() override;
	/*end HighlightInterface*/

	/*Combat Interface*/
	FORCEINLINE virtual int32 GetPlayerLevel_Implementation() override {return Level;};
	virtual void Die(const FVector& InDeathImpulse) override;
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

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float LifeSpan=5.0f;

	UPROPERTY(BlueprintReadWrite,Category="Combat")
	TObjectPtr<AActor> CombatTarget;
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttribute() const override;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharacterDefaultClass")
	int32 Level=1;



	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="CharacterDefaultClass")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<APhosphorAIController> PhosphorAIController;
	
};
