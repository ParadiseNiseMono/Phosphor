// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PhosphorPlayerState.generated.h"

class ULevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChenged, int32 /*StateValue*/)

/**
 * 
 */
class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class PHOSPHOR_API APhosphorPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APhosphorPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FOnPlayerStateChenged OnXPChangedDelegate;
	FOnPlayerStateChenged OnLevelChangedDelegate;

	FORCEINLINE int32 GetPlayerLevel()const {return Level;}
	FORCEINLINE int32 GetPlayerXP()const {return XP;}
	
	void SetXP(const int32 NewXP);
	void SetLevel(const int32 NewLevel);
	
	void AddToXP(const int32 AddXP);
	void AddLevel(const int32 AddLevel);
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere,Replicated,ReplicatedUsing=OnRep_Level)
	int32 Level=1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(VisibleAnywhere,Replicated,ReplicatedUsing=OnRep_XP)
	int32 XP=0;

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

};
