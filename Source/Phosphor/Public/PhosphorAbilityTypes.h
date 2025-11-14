#pragma once

#include "GameplayEffectTypes.h"
#include "PhosphorAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContext = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;
};

USTRUCT(BlueprintType)
struct FPhosphorGameplayEffectContext :public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockHit() const {return bIsBlockHit;}
	bool IsCriticalHit() const {return bIsCriticalHit;}

	void SetIsCriticalHit(bool InIsCriticalHit) {bIsCriticalHit=InIsCriticalHit;}
	void SetIsBlockHit(bool InIsBlockHit) {bIsBlockHit=InIsBlockHit;}
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::GetScriptStruct();
	}

	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FPhosphorGameplayEffectContext* Duplicate() const
	{
		FPhosphorGameplayEffectContext* NewContext = new FPhosphorGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
protected:
	bool bIsBlockHit=false;
	bool bIsCriticalHit=false;
};
template<> // 表示這是一個模板特化
struct TStructOpsTypeTraits<FPhosphorGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FPhosphorGameplayEffectContext> // 為 FAuraGameplayEffectContext 特化
{
	enum
	{
		// 【必須】告訴系統：這個 Struct 有自訂的 NetSerialize 函式
		WithNetSerialize = true,
		// 【推薦】告訴系統：這個 Struct 可以被安全地複製 (需要配合 Duplicate 或拷貝建構函式)
		WithCopy=true
	};
};
