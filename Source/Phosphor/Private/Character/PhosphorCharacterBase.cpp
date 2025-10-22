// copyright Paradise_NiseMono


#include "Character/PhosphorCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"

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

FVector APhosphorCharacterBase::GetCombatSocketLocation()
{
	check(Weapon)
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void APhosphorCharacterBase::InitAbilityActorInfo()
{
}

void APhosphorCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass,const float Level) const
{
	check(GetAbilitySystemComponent());
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle=GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle=GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,Level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void APhosphorCharacterBase::InitializeDefaultAttribute() const
{
	ApplyEffectToSelf(DefaultPrimaryAttribute,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttribute,1.f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.f);
}

void APhosphorCharacterBase::AddCharacterAbilities()
{
	UPhosphorAbilitySystemComponent* ASC=CastChecked<UPhosphorAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	
	ASC->AddCharacterAbilities(StartUpAbilities);
}
