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
};
