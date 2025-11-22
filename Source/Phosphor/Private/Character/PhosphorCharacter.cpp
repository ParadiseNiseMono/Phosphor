// copyright Paradise_NiseMono


#include "Character/PhosphorCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/LoadScreenSaveGame.h"
#include "Game/PhosphorGameInstance.h"
#include "Game/PhosphorGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PhosphorPlayerController.h"
#include "Player/PhosphorPlayerState.h"
#include "UI/HUD/PhosphorHUD.h"

APhosphorCharacter::APhosphorCharacter()
{
	CameraBoom=CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest=false;

	TopDownCameraComponent=CreateDefaultSubobject<UCameraComponent>("TopDownCamera");
	TopDownCameraComponent->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	LevelUpNiagaraComponent=CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(RootComponent);
	LevelUpNiagaraComponent->bAutoActivate=false;
	
	GetCharacterMovement()->bOrientRotationToMovement=true;
	GetCharacterMovement()->RotationRate=FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane=true;
	GetCharacterMovement()->bSnapToPlaneAtStart=true;

	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;

	CharacterClass=ECharacterClass::Elemantalist;
}

void APhosphorCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//init ability actor for the server
	InitAbilityActorInfo();

	LoadProgress();
}

void APhosphorCharacter::LoadProgress()
{
	APhosphorGameModeBase* PhosphorGameModeBase = Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (PhosphorGameModeBase)
	{
		ULoadScreenSaveGame* SaveData = PhosphorGameModeBase->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		if (SaveData->bFirstTimeLogIn)
		{
			InitializeDefaultAttribute();
			AddCharacterAbilities();
		}
		else
		{
			if (UPhosphorAbilitySystemComponent* PhosphorASC = Cast<UPhosphorAbilitySystemComponent>(AbilitySystemComponent))
			{
				PhosphorASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}
			
			if (APhosphorPlayerState* PhosphorPlayerState = Cast<APhosphorPlayerState>(GetPlayerState()))
			{
				PhosphorPlayerState->SetLevel(SaveData->PlayerLevel);
				PhosphorPlayerState->SetXP(SaveData->XP);
				PhosphorPlayerState->SetAttributePoint(SaveData->AttributePoint);
				PhosphorPlayerState->SetSpellPoint(SaveData->SpellPoint);
			}
			UPhosphorAbilitySystemLibrary::InitializeDefaultAbilitiesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

void APhosphorCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//init ability actor for the client
	InitAbilityActorInfo();
}

void APhosphorCharacter::OnRep_Stunned()
{
	if (UPhosphorAbilitySystemComponent* PhosphorASC = Cast<UPhosphorAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FPhosphorGameplayTags& GameplayTags = FPhosphorGameplayTags::Get();
		FGameplayTagContainer BlockTags;
		BlockTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			PhosphorASC->AddLooseGameplayTags(BlockTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			PhosphorASC->RemoveLooseGameplayTags(BlockTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void APhosphorCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void APhosphorCharacter::AddToXP_Implementation(int32 InXP)
{
	APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	PhosphorPlayerState->AddToXP(InXP);
}

void APhosphorCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void APhosphorCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation=TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraLocation=LevelUpNiagaraComponent->GetComponentLocation();

		const FRotator ToCameraRotation=(CameraLocation-NiagaraLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}
int32 APhosphorCharacter::GetXP_Implementation() const
{
	const APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	return PhosphorPlayerState->GetPlayerXP();
}

int32 APhosphorCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	return PhosphorPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 APhosphorCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	return PhosphorPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

int32 APhosphorCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	return PhosphorPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
}

void APhosphorCharacter::AddToPlayerLevel_Implementation(int32 InLevel)
{
	APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	PhosphorPlayerState->AddToLevel(InLevel);

	if (UPhosphorAbilitySystemComponent* PhosphorASC = Cast<UPhosphorAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		PhosphorASC->UpdateAbilityStatus(PhosphorPlayerState->GetPlayerLevel());
	}
}

void APhosphorCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	PhosphorPlayerState->AddToAttributePoint(InAttributePoints);
}

void APhosphorCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	PhosphorPlayerState->AddToSpellPoint(InSpellPoints);
}

int32 APhosphorCharacter::GetAttributePoints_Implementation()
{
	APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	return PhosphorPlayerState->GetPlayerAttributePoint();
}

int32 APhosphorCharacter::GetSpellPoints_Implementation()
{
	APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	return PhosphorPlayerState->GetPlayerSpellPoint();
}

void APhosphorCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (APhosphorPlayerController* PhosphorPlayerController = Cast<APhosphorPlayerController>(GetController()))
	{
		PhosphorPlayerController->ShowMagicCircle(DecalMaterial);
		PhosphorPlayerController->bShowMouseCursor = false;
	}
}

void APhosphorCharacter::HideMagicCircle_Implementation()
{
	if (APhosphorPlayerController* PhosphorPlayerController = Cast<APhosphorPlayerController>(GetController()))
	{
		PhosphorPlayerController->HideMagicCircle();
		PhosphorPlayerController->bShowMouseCursor = true;
	}
}

void APhosphorCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	APhosphorGameModeBase* PhosphorGameModeBase = Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (PhosphorGameModeBase)
	{
		ULoadScreenSaveGame* SaveData = PhosphorGameModeBase->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		SaveData->PlayerStartTag = CheckpointTag;
		if (APhosphorPlayerState* PhosphorPlayerState = Cast<APhosphorPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = PhosphorPlayerState->GetPlayerLevel();
			SaveData->XP = PhosphorPlayerState->GetPlayerXP();
			SaveData->SpellPoint = PhosphorPlayerState->GetPlayerSpellPoint();
			SaveData->AttributePoint = PhosphorPlayerState->GetPlayerAttributePoint();
		}
		SaveData->Strength = UPhosphorAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UPhosphorAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UPhosphorAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UPhosphorAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

		SaveData->bFirstTimeLogIn = false;
		if (!HasAuthority()) return;

		UPhosphorAbilitySystemComponent* PhosphorASC = Cast<UPhosphorAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveData->SaveAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, PhosphorASC, SaveData](const FGameplayAbilitySpec& Spec)
		{
			const FGameplayTag AbilityTag = PhosphorASC->GetAbilityTagFromSpec(Spec);
			UAbilityInfo* AbilityInfo = UPhosphorAbilitySystemLibrary::GetAbilityInfo(this);
			FPhosphorAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			
			FSaveAbility SavedAbility;
			SavedAbility.GameplayAbility = Info.Ability;
			SavedAbility.AbilityLevel = Spec.Level;
			SavedAbility.AbilitySlot = PhosphorASC->GetSlotFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = PhosphorASC->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityType;

			SaveData->SaveAbilities.AddUnique(SavedAbility);
		});
		PhosphorASC->ForEachAbility(SaveAbilityDelegate);

		PhosphorGameModeBase->SaveInGameProgressData(SaveData);
	}
}

int32 APhosphorCharacter::GetPlayerLevel_Implementation()
{
	const APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	return PhosphorPlayerState->GetPlayerLevel();
}

void APhosphorCharacter::InitAbilityActorInfo()
{
	APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	PhosphorPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PhosphorPlayerState,this);
	Cast<UPhosphorAbilitySystemComponent>(PhosphorPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent=PhosphorPlayerState->GetAbilitySystemComponent();
	AttributeSet=PhosphorPlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(
		FPhosphorGameplayTags::Get().Debuff_Stun,
		EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APhosphorCharacter::StunTagChanged);

	if (APhosphorPlayerController* PhosphorPlayerController=Cast<APhosphorPlayerController>(GetController()))
	{
		if (APhosphorHUD* PhosphorHUD=Cast<APhosphorHUD>(PhosphorPlayerController->GetHUD()))
		{
			PhosphorHUD->InitOverlay(PhosphorPlayerController,PhosphorPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
}





