// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorBeamSpell.h"

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

void UPhosphorBeamSpell::StoreOwnerController()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
	}
}
