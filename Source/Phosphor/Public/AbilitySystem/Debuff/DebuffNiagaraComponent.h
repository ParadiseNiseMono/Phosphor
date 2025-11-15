// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class PHOSPHOR_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:

	UDebuffNiagaraComponent();
	
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag = FGameplayTag();

protected:
	virtual void BeginPlay() override;
	void DebuffTagChanged(const FGameplayTag InDebuffTag,int32 NewCount);

	UFUNCTION()
	void OnOwnerDaeth(AActor* DeadActor);
};
