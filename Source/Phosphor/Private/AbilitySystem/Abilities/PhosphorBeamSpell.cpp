// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorBeamSpell.h"

#include "GameFramework/Character.h"

void UPhosphorBeamSpell::StoreMouseDataInfo(const FHitResult& MouseHitResult)
{
	if (MouseHitResult.bBlockingHit)
	{
		MouseHitLocation = MouseHitResult.ImpactPoint;
		MouseHitActor = MouseHitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UPhosphorBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}
