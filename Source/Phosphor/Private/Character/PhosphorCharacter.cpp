// copyright Paradise_NiseMono


#include "Character/PhosphorCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PhosphorPlayerState.h"

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
	AbilitySystemComponent=PhosphorPlayerState->GetAbilitySystemComponent();
	AttributeSet=PhosphorPlayerState->GetAttributeSet();
}
