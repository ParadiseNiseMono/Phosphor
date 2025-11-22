// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PhosphorPlayerState.generated.h"

class ULevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, int32 /*StateValue*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StateValue*/, bool /*bLevelUp*/)
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
	
	FOnPlayerStateChanged OnXPChangedDelegate;
	FOnLevelChanged OnLevelChangedDelegate;
	FOnPlayerStateChanged OnAttributePointChangedDelegate;
	FOnPlayerStateChanged OnSpellPointChangedDelegate;

	FORCEINLINE int32 GetPlayerLevel()const {return Level;}
	FORCEINLINE int32 GetPlayerXP()const {return XP;}
	FORCEINLINE int32 GetPlayerAttributePoint()const {return AttributePoint;}
	FORCEINLINE int32 GetPlayerSpellPoint()const {return SpellPoint;}
	
	void SetXP(const int32 NewXP);
	void SetLevel(const int32 NewLevel);
	void SetAttributePoint(const int32 NewAttributePoint);
	void SetSpellPoint(const int32 NewSpellPoint);
	
	void AddToXP(const int32 AddXP);
	void AddToLevel(const int32 AddLevel);
	void AddToAttributePoint(const int32 AddAttributePoint);
	void AddToSpellPoint(const int32 AddSpellPoint);
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere,Replicated,ReplicatedUsing=OnRep_Level)
	int32 Level=1;

	UPROPERTY(VisibleAnywhere,Replicated,ReplicatedUsing=OnRep_XP)
	int32 XP=0;

	UPROPERTY(VisibleAnywhere,Replicated,ReplicatedUsing=OnRep_AttributePoint)
	int32 AttributePoint=0;

	UPROPERTY(VisibleAnywhere,Replicated,ReplicatedUsing=OnRep_SpellPoint)
	int32 SpellPoint=0;	

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);

	UFUNCTION()
	void OnRep_SpellPoint(int32 OldSpellPoint);

	
};
