// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "PhosphorWidgetController.generated.h"


class UAbilityInfo;
class UPhosphorAttributeSet;
class UPhosphorAbilitySystemComponent;
class APhosphorPlayerState;
class APhosphorPlayerController;
class UAttributeSet;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature,const FPhosphorAbilityInfo&, Info);


USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC),AttributeSet(AS){}

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController=nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState=nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent=nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet=nullptr;
};

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	void SetControllerParams(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();

	UPROPERTY(BlueprintAssignable, Category="GAS|Message")
	FAbilityInfoSignature AbilityInfoDelegate;

	void BroadcastAbilityInfo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APhosphorPlayerController> PhosphorPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APhosphorPlayerState> PhosphorPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UPhosphorAbilitySystemComponent> PhosphorAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UPhosphorAttributeSet> PhosphorAttributeSet;

	APhosphorPlayerController* GetPhosphorPC();

	APhosphorPlayerState* GetPhosphorPS();

	UPhosphorAbilitySystemComponent* GetPhosphorASC();

	UPhosphorAttributeSet* GetPhosphorAS();
};
