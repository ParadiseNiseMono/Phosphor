// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PhosphorAbilityTypes.h"
#include "PhosphorGameplayTags.h"
#include "Game/PhosphorGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PhosphorPlayerState.h"
#include "UI/HUD/PhosphorHUD.h"
#include "UI/WidgetController/PhosphorWidgetController.h"

bool UPhosphorAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,FWidgetControllerParams& OutWCParams,APhosphorHUD*& OutPhosphorHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutPhosphorHUD=Cast<APhosphorHUD>(PC->GetHUD());
		if (OutPhosphorHUD)
		{
			APhosphorPlayerState* PS=PC->GetPlayerState<APhosphorPlayerState>();
			UAbilitySystemComponent* ASC=PS->GetAbilitySystemComponent();
			UAttributeSet* AS=PS->GetAttributeSet();

			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet=AS;
			OutWCParams.PlayerState=PS;
			OutWCParams.PlayerController=PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UPhosphorAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APhosphorHUD* PhosphorHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject,WCParams,PhosphorHUD))
	{
		return  PhosphorHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UPhosphorAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APhosphorHUD* PhosphorHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject,WCParams,PhosphorHUD))
	{
		return  PhosphorHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UPhosphorAbilitySystemLibrary::GetSpellMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APhosphorHUD* PhosphorHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject,WCParams,PhosphorHUD))
	{
		return  PhosphorHUD->GetSpellMenuWidgetController(WCParams);
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
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

int32 UPhosphorAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo=GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo==nullptr) return 0;

	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward=Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

UCharacterClassInfo* UPhosphorAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	APhosphorGameModeBase* PhosphorGameModeBase= Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (PhosphorGameModeBase==nullptr) return nullptr;

	return PhosphorGameModeBase->CharacterClassInfo;
}

UAbilityInfo* UPhosphorAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	APhosphorGameModeBase* PhosphorGameModeBase= Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (PhosphorGameModeBase==nullptr) return nullptr;

	return PhosphorGameModeBase->AbilityInfo;
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

FGameplayEffectContextHandle UPhosphorAbilitySystemLibrary::ApplyDamageEffect(
	const FDamageEffectParams& DamageEffectParams)
{
	const FPhosphorGameplayTags GameplayTags = FPhosphorGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle ContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
		DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return ContextHandle;
	
}


