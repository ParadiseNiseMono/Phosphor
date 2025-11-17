// copyright Paradise_NiseMono


#include "AbilitySystem/PhosphorAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/PhosphorGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Interaction/PlayerInterface.h"
#include "Phosphor/PhosphorLogChannels.h"

void UPhosphorAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UPhosphorAbilitySystemComponent::ClientEffectApplied);
}

void UPhosphorAbilitySystemComponent::AddCharacterAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	for (const TSubclassOf<UGameplayAbility> GameAbility : StartUpAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameAbility,1);
		if (const UPhosphorGameplayAbility* PhosphorGameplayAbility=Cast<UPhosphorGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(PhosphorGameplayAbility->StartUpTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FPhosphorGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven=true;
	AbilitiesGivenDelegate.Broadcast();
}

void UPhosphorAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartUpPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> GameAbility : StartUpPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameAbility,1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UPhosphorAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
				const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
				FPredictionKey OriginalPredictionKey = ActivationInfo.GetActivationPredictionKey();
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, OriginalPredictionKey);
			}
		}
	}
}

void UPhosphorAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UPhosphorAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
			const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
			FPredictionKey OriginalPredictionKey = ActivationInfo.GetActivationPredictionKey();
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, OriginalPredictionKey);
		}
	}
}

void UPhosphorAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(Spec))
		{
			UE_LOG(LogPhosphor, Error, TEXT("Failed to execute delegate in %hs"),__FUNCTION__);
		}
	}
}

FGameplayTag UPhosphorAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& Spec)
{
	if (Spec.Ability)
	{
		for (FGameplayTag Tag:Spec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Abilities")))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UPhosphorAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& Spec)
{
	for(FGameplayTag Tag:Spec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag")))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UPhosphorAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& Spec)
{
	for (FGameplayTag StatusTag : Spec.GetDynamicSpecSourceTags())
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag("Abilities.Status")))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UPhosphorAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UPhosphorAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

bool UPhosphorAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, Slot)) return false;
	}
	return true;
}

bool UPhosphorAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.GetDynamicSpecSourceTags().HasTagExact(Slot);
}

bool UPhosphorAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.GetDynamicSpecSourceTags().HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

FGameplayAbilitySpec* UPhosphorAbilitySystemComponent::GetSpecFromSlotTag(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, Slot)) return &AbilitySpec;
	}
	return nullptr;
}

bool UPhosphorAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& Spec) const
{
	const UAbilityInfo* AbilityInfo = UPhosphorAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(Spec);
	const FPhosphorAbilityInfo& Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	const FGameplayTag AbilityType = Info.AbilityType;
	return AbilityType.MatchesTagExact(FPhosphorGameplayTags::Get().Abilities_Type_Passive);
}

void UPhosphorAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.GetDynamicSpecSourceTags().AddTag(Slot);
}

void UPhosphorAbilitySystemComponent::MulticastActivePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bActivate)
{
	ActivatePassiveEffect.Broadcast(AbilityTag, bActivate);
}

FGameplayAbilitySpec* UPhosphorAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec:GetActivatableAbilities())
	{
		for (FGameplayTag Tag:Spec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &Spec;
			}
		}
	}
	return nullptr;
}

void UPhosphorAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UPhosphorAbilitySystemComponent::UpdateAbilityStatus(const int32 Level)
{
	UAbilityInfo* AbilityInfo=UPhosphorAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FPhosphorAbilityInfo Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Info.Ability,1);
			Spec.GetDynamicSpecSourceTags().AddTag(FPhosphorGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(Spec);
			MarkAbilitySpecDirty(Spec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FPhosphorGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

void UPhosphorAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 100.0f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),AttributeTag,Payload);

	IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(),-1);
}

void UPhosphorAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(),-1);
		}
		
		const FPhosphorGameplayTags GameplayTags = FPhosphorGameplayTags::Get();
		FGameplayTag StatusTag = GetStatusFromSpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Unlocked);
			StatusTag = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityStatus(AbilityTag, StatusTag, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UPhosphorAbilitySystemComponent::SeverEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FPhosphorGameplayTags GameplayTags = FPhosphorGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);

		const bool bStatusValid = Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_Unlocked;
		if (bStatusValid)
		{
			if (!SlotIsEmpty(Slot))
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecFromSlotTag(Slot);
				if (SpecWithSlot)
				{
					//Is that ability the same as this ability? If so,return early.
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
						return;
					}
					if (IsPassiveAbility(*SpecWithSlot))
					{
						MulticastActivePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlot), false);
						DeactivatePassiveAbility.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot));
					}

					ClearSlot(SpecWithSlot);
				}
			}
			if (!AbilityHasAnySlot(*AbilitySpec)) //Ability doesn't yet have a Slot .(Is not Active)
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					MulticastActivePassiveEffect(AbilityTag, true);
					TryActivateAbility(AbilitySpec->Handle);
				}
			}
			AssignSlotToAbility(*AbilitySpec, Slot);
			
			if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
			{
				AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Unlocked);
				AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Equipped);	
			}
			MarkAbilitySpecDirty(*AbilitySpec);
			ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
		}
	}
	
}

void UPhosphorAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

bool UPhosphorAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag,
                                                                 FString& OutDescription, FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UPhosphorGameplayAbility* PhosphorGameplayAbility = Cast<UPhosphorGameplayAbility>(Spec->Ability))
		{
			OutDescription = PhosphorGameplayAbility->GetDescription(Spec->Level);
			OutNextLevelDescription = PhosphorGameplayAbility->GetNextLevelDescription(Spec->Level + 1);
			return true;
		}
	}
	const UAbilityInfo* AbilityInfo = UPhosphorAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FPhosphorGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UPhosphorGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoByTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

void UPhosphorAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Slot);
}

void UPhosphorAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
	
}

bool UPhosphorAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : AbilitySpec->GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

void UPhosphorAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven=true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UPhosphorAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UPhosphorAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                         const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
