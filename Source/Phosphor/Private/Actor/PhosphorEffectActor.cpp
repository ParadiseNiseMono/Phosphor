// copyright Paradise_NiseMono


#include "Actor/PhosphorEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "Components/SphereComponent.h"

APhosphorEffectActor::APhosphorEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneComponent"));
}

void APhosphorEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void APhosphorEffectActor::ApplyEffectToTarget(AActor* ActorToTarget, TSubclassOf<UGameplayEffect> EffectToApply)
{
	UAbilitySystemComponent* TargetASC= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorToTarget);
	if (TargetASC==nullptr)return;

	check(EffectToApply);
	FGameplayEffectContextHandle EffectContextHandle= TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectToApply,ActorLevel,EffectContextHandle);
	
	const FActiveGameplayEffectHandle ActiveEventHandle=TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite=EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy==EGameplayEffectDurationType::Infinite;
	if (bIsInfinite&&InfiniteEffectRemovalPolicy==EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEventHandles.Add(ActiveEventHandle,TargetASC);
	}
}

void APhosphorEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
}

void APhosphorEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy==EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (TargetASC==nullptr)return;

		TArray<FActiveGameplayEffectHandle> HandlesToBeRemove;
		for (auto HandlePair : ActiveEventHandles)
		{
			TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
			HandlesToBeRemove.Add(HandlePair.Key);
		}
		for (auto& Handle : HandlesToBeRemove)
		{
			ActiveEventHandles.FindAndRemoveChecked(Handle);
		}
	}
}



