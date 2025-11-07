// copyright Paradise_NiseMono


#include "Character/PhosphorCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PhosphorPlayerController.h"
#include "Player/PhosphorPlayerState.h"
#include "UI/HUD/PhosphorHUD.h"

APhosphorCharacter::APhosphorCharacter()
{
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
	AddCharacterAbilities();
}

void APhosphorCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//init ability actor for the client
	InitAbilityActorInfo();
}

void APhosphorCharacter::AddToXP_Implementation(int32 InXP)
{
	APhosphorPlayerState* PhosphorPlayerState=GetPlayerState<APhosphorPlayerState>();
	check(PhosphorPlayerState);
	PhosphorPlayerState->AddToXP(InXP);
}

void APhosphorCharacter::LevelUp_Implementation()
{
	
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
	PhosphorPlayerState->AddLevel(InLevel);
}

void APhosphorCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	//TODO: Add AttributePoints to Player State
}

void APhosphorCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	//TODO: Add SpellPoints to Player State
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

	if (APhosphorPlayerController* PhosphorPlayerController=Cast<APhosphorPlayerController>(GetController()))
	{
		if (APhosphorHUD* PhosphorHUD=Cast<APhosphorHUD>(PhosphorPlayerController->GetHUD()))
		{
			PhosphorHUD->InitOverlay(PhosphorPlayerController,PhosphorPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
	InitializeDefaultAttribute();
}

