// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PhosphorPlayerController.generated.h"

struct FGameplayTag;
class UPhosphorInputConfig;
/**
 * 
 */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UPhosphorAbilitySystemComponent;

class USplineComponent;
UCLASS()
class PHOSPHOR_API APhosphorPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	APhosphorPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
protected:
	virtual void BeginPlay()override;
	virtual void SetupInputComponent()override;
private:
	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PhosphorContext;

	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftKeyPressed(){bShiftKeyPressed = true;};
	void ShiftKeyReleased(){bShiftKeyPressed = false;};
	bool bShiftKeyPressed=false;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
	FHitResult HitResult;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UPhosphorInputConfig> PhosphorInputConfig;

	UPROPERTY()
	TObjectPtr<UPhosphorAbilitySystemComponent> PhosphorAbilitySystemComponent;

	UPhosphorAbilitySystemComponent* GetASC();

	
	FVector CachedDestination=FVector::ZeroVector;
	float FollowTime=0.0f;
	float ShortPressThreshold=0.5f;
	bool bAutoRunning=false;
	bool bTargeting=false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius=50.0f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
};
