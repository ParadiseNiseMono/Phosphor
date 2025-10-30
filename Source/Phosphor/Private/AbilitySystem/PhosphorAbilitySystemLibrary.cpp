// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"

#include "PhosphorAbilityTypes.h"
#include "Game/PhosphorGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PhosphorPlayerState.h"
#include "UI/HUD/PhosphorHUD.h"
#include "UI/WidgetController/PhosphorWidgetController.h"

UOverlayWidgetController* UPhosphorAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APhosphorHUD* PhosphorHUD=Cast<APhosphorHUD>(PC->GetHUD()))
		{
			APhosphorPlayerState* PS=PC->GetPlayerState<APhosphorPlayerState>();
			UAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
			UAttributeSet* AS=PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return  PhosphorHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UPhosphorAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APhosphorHUD* PhosphorHUD=Cast<APhosphorHUD>(PC->GetHUD()))
		{
			APhosphorPlayerState* PS=PC->GetPlayerState<APhosphorPlayerState>();
			UAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
			UAttributeSet* AS=PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return  PhosphorHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

void UPhosphorAbilitySystemLibrary::InitializeDefaultAbilities(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor=ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo=CharacterClassInfo->GetClassDefaultInfo(CharacterClass);


	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level,PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,Level,SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level,VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UPhosphorAbilitySystemLibrary::GiveStartUpAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo==nullptr) return;
	for (TSubclassOf<UGameplayAbility> GameAbility:CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameAbility,1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& CharacterClassDefaultInfo= CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass: CharacterClassDefaultInfo.StartUpAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UPhosphorAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	APhosphorGameModeBase* PhosphorGameModeBase= Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (PhosphorGameModeBase==nullptr) return nullptr;

	return PhosphorGameModeBase->CharacterClassInfo;
}

bool UPhosphorAbilitySystemLibrary::IsBlockHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext=static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->IsBlockHit();
	}
	return false;
}

bool UPhosphorAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext=static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

void UPhosphorAbilitySystemLibrary::SetIsBlockHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsBlockHit)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext=static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetIsBlockHit(bInIsBlockHit);
	}
}

void UPhosphorAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsCriticalHit)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext=static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UPhosphorAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UPhosphorAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bFirstIsPlayer=FirstActor->ActorHasTag("Player");
	const bool bSecondIsPlayer=SecondActor->ActorHasTag("Player");
	const bool bFirstIsEnemy=FirstActor->ActorHasTag("Enemy");
	const bool bSecondIsEnemy=SecondActor->ActorHasTag("Enemy");

	return !((bFirstIsPlayer&&bSecondIsPlayer)||(bFirstIsEnemy&&bSecondIsEnemy));
}
