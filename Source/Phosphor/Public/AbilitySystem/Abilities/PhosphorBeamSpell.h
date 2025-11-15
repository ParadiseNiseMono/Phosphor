// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PhosphorDamageGameplayAbility.h"
#include "PhosphorBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorBeamSpell : public UPhosphorDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& MouseHitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerController();
protected:

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
};
