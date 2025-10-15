// copyright Paradise_NiseMono


#include "Character/PhosphorCharacterBase.h"

APhosphorCharacterBase::APhosphorCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* APhosphorCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APhosphorCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

