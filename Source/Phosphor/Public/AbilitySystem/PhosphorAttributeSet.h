// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PhosphorAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()
	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category ="Effect Properties")
	TObjectPtr<UAbilitySystemComponent> SourceASC=nullptr;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category ="Effect Properties")
	TObjectPtr<AActor> SourceAvatarActor=nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category ="Effect Properties")
	TObjectPtr<AController> SourceController=nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category ="Effect Properties")
	TObjectPtr<ACharacter> SourceCharacter=nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category ="Effect Properties")
	TObjectPtr<UAbilitySystemComponent> TargetASC=nullptr;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category ="Effect Properties")
	TObjectPtr<AActor> TargetAvatarActor=nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category ="Effect Properties")
	TObjectPtr<AController> TargetController=nullptr;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category ="Effect Properties")
	TObjectPtr<ACharacter> TargetCharacter=nullptr;
	
};


/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPhosphorAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const  FGameplayEffectModCallbackData& Data) override;

	/*
	 * Primary Attributes
	 */
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Strength,Category="Primary Attritubes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,Strength);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Intelligence,Category="Primary Attritubes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,Intelligence);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Resilience,Category="Primary Attritubes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,Resilience);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Vigor,Category="Primary Attritubes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,Vigor);
	/*
	 * Vital Attributes
	 */

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health,Category="Vital Attritubes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,Health);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxHealth,Category="Vital Attritubes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,MaxHealth);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Mana,Category="Vital Attritubes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,Mana);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxMana,Category="Vital Attritubes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,MaxMana);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
protected:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props) const;
};
