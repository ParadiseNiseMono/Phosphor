// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PhosphorPlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue; 
UCLASS()
class PHOSPHOR_API APhosphorPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	APhosphorPlayerController();
protected:
	virtual void BeginPlay()override;
	virtual void SetupInputComponent()override;
private:
	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PhosphorContext;

	UPROPERTY(EditAnyWhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void move(const FInputActionValue& InputActionValue);
};
