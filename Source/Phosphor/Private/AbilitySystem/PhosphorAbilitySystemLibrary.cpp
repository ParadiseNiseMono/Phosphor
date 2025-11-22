// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PhosphorAbilityTypes.h"
#include "PhosphorGameplayTags.h"
#include "Game/LoadScreenSaveGame.h"
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

void UPhosphorAbilitySystemLibrary::InitializeDefaultAbilitiesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	const FPhosphorGameplayTags& GameplayTags = FPhosphorGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceAvatarActor);
	

	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Resilience, SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vigor, SaveGame->Vigor);

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite,1.f,SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,1.f,VitalAttributesContextHandle);
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
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

void UPhosphorAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, const bool bIsRadial, const float InnerRadius, const float OuterRadius, const FVector& Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UPhosphorAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams,
	FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.0f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
	}
}

void UPhosphorAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams,
	FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.0f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
}

void UPhosphorAbilitySystemLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams,
	UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}

UCharacterClassInfo* UPhosphorAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	APhosphorGameModeBase* PhosphorGameModeBase = Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (PhosphorGameModeBase == nullptr) return nullptr;

	return PhosphorGameModeBase->CharacterClassInfo;
}

UAbilityInfo* UPhosphorAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	APhosphorGameModeBase* PhosphorGameModeBase = Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (PhosphorGameModeBase == nullptr) return nullptr;

	return PhosphorGameModeBase->AbilityInfo;
}

bool UPhosphorAbilitySystemLibrary::IsBlockHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->IsBlockHit();
	}
	return false;
}

bool UPhosphorAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

bool UPhosphorAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UPhosphorAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UPhosphorAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UPhosphorAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UPhosphorAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		if (PhosphorGameplayEffectContext->GetDamageType().IsValid())
		{
			return *PhosphorGameplayEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UPhosphorAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UPhosphorAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UPhosphorAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->IsRadialDamage();
	}
	return false;
}

float UPhosphorAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UPhosphorAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UPhosphorAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<const FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		return PhosphorGameplayEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UPhosphorAbilitySystemLibrary::SetIsBlockHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsBlockHit)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetIsBlockHit(bInIsBlockHit);
	}
}

void UPhosphorAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsCriticalHit)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UPhosphorAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& ContextHandle,
	bool bInIsSuccessfulDebuff)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UPhosphorAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& ContextHandle, float InDamage)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetDebuffDamage(InDamage);
	}
}

void UPhosphorAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& ContextHandle, float InDuration)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetDebuffDuration(InDuration);
	}
}

void UPhosphorAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& ContextHandle, float InFrequency)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UPhosphorAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& ContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		PhosphorGameplayEffectContext->SetDamageType(DamageType);
	}
}

void UPhosphorAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& ContextHandle,
	const FVector& InImpulse)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UPhosphorAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& ContextHandle,
	const FVector& InForce)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetKnockbackForce(InForce);
	}
}

void UPhosphorAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& ContextHandle,
	bool bInIsRadialDamage)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UPhosphorAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& ContextHandle,
	float InInnerRadius)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UPhosphorAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& ContextHandle,
	float InOuterRadius)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UPhosphorAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& ContextHandle,
	const FVector& InOrigin)
{
	if (FPhosphorGameplayEffectContext* PhosphorGameplayEffectContext = static_cast<FPhosphorGameplayEffectContext*>(ContextHandle.Get()))
	{
		PhosphorGameplayEffectContext->SetRadialDamageOrigin(InOrigin);
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

void UPhosphorAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors,
	TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}
	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;
	
	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestTarget = nullptr;
		for (AActor* Actor : ActorsToCheck)
		{
			double Distance = (Actor->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestTarget = Actor;
			}
		}
		ActorsToCheck.Remove(ClosestTarget);
		OutClosestTargets.AddUnique(ClosestTarget);
		NumTargetsFound++;
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
	const FPhosphorGameplayTags& GameplayTags = FPhosphorGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle ContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(ContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(ContextHandle, DamageEffectParams.KnockbackForce);

	SetIsRadialDamage(ContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(ContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(ContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(ContextHandle, DamageEffectParams.RadialDamageOrigin);
	

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
		DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	return ContextHandle;
}

TArray<FRotator> UPhosphorAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);

	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction=LeftOfSpread.RotateAngleAxis(i*DeltaSpread,Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UPhosphorAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)	
{
	TArray<FVector> Vectors;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);

	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction=LeftOfSpread.RotateAngleAxis(i*DeltaSpread,Axis);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}


