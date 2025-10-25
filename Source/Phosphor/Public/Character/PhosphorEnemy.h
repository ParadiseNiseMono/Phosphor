// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Character/PhosphorCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "PhosphorEnemy.generated.h"

class UWidgetComponent;
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

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallBackTag,int32 NewCount);

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReacting=false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed=300.0f;
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttribute() const override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharacterDefaultClass")
	int32 Level=1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharacterDefaultClass")
	ECharacterClass CharacterClass=ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="CharacterDefaultClass")
	TObjectPtr<UWidgetComponent> HealthBar;

	
};
