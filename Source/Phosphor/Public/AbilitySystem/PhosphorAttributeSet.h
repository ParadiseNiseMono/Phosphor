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
	 * Secondary Attributes
	 */
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Armor,Category="Secondary Attritubes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,Armor);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_ArmorPenetration,Category="Secondary Attritubes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,ArmorPenetration);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_BlockChance,Category="Secondary Attritubes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,BlockChance);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_CriticalHitChance,Category="Secondary Attritubes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,CriticalHitChance);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_CriticalHitDamage,Category="Secondary Attritubes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_CriticalHitResistance,Category="Secondary Attritubes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_HealthRegeneration,Category="Secondary Attritubes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,HealthRegeneration);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_ManaRegeneration,Category="Secondary Attritubes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,ManaRegeneration);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxHealth,Category="Secondary Attritubes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,MaxHealth);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxMana,Category="Secondary Attritubes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,MaxMana);
	
	/*
	 * Vital Attributes
	 */

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health,Category="Vital Attritubes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,Health);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Mana,Category="Vital Attritubes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPhosphorAttributeSet,Mana);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

protected:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props) const;
};
