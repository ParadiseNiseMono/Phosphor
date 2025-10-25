// copyright Paradise_NiseMono


#include "Character/PhosphorCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Phosphor/Phosphor.h"

APhosphorCharacterBase::APhosphorCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* APhosphorCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* APhosphorCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
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
