// copyright Paradise_NiseMono


#include "Character/PhosphorCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
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
}

void APhosphorCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//init ability actor for the server
	InitAbilityActorInfo();
}

void APhosphorCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//init ability actor for the client
	InitAbilityActorInfo();
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
	InitializePrimaryAttributes();
}
