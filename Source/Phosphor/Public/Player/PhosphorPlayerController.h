// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PhosphorPlayerController.generated.h"

class IHighlightInterface;
class AMagicCircle;
class UNiagaraSystem;
class UDamageTextComponent;
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

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

class USplineComponent;
UCLASS()
class PHOSPHOR_API APhosphorPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	APhosphorPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client,Reliable)
	void ShowDamageNumber(const float Damage,ACharacter* Target,bool bBlockHit,bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
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
	
	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> ThisActor;
	FHitResult CursorHit;

	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UPhosphorInputConfig> PhosphorInputConfig;

	UPROPERTY()
	TObjectPtr<UPhosphorAbilitySystemComponent> PhosphorAbilitySystemComponent;

	UPhosphorAbilitySystemComponent* GetASC();

	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.0f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;

	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius=50.0f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircleLocation();
};
