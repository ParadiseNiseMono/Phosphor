// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhosphorCharacterBase.generated.h"

UCLASS()
class PHOSPHOR_API APhosphorCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APhosphorCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
