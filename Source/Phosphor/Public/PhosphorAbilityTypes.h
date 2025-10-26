#pragma once

#include "GameplayEffectTypes.h"
#include "PhosphorAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FPhosphorGameplayEffectContext :public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsBlockHit() const {return bIsBlockHit;}
	FORCEINLINE bool IsCriticalHit() const {return bIsCriticalHit;}

	FORCEINLINE void SetIsCriticalHit(bool InIsCriticalHit) {bIsCriticalHit=InIsCriticalHit;}
	FORCEINLINE void SetIsBlockHit(bool InIsBlockHit) {bIsBlockHit=InIsBlockHit;}
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
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
