// copyright Paradise_NiseMono


#include "Character/PhosphorCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
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

void APhosphorCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

TArray<FTaggedMontage> APhosphorCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* APhosphorCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage APhosphorCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage:AttackMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(MontageTag))
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 APhosphorCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void APhosphorCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass APhosphorCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

void APhosphorCharacterBase::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(),GetActorRotation());
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead=true;
}

void APhosphorCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector APhosphorCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FPhosphorGameplayTags& GameplayTags=FPhosphorGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
	
}

bool APhosphorCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* APhosphorCharacterBase::GetAvatar_Implementation()
{
	return this;
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

void APhosphorCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MaterialInstanceDynamic=UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0, MaterialInstanceDynamic);
		StartDissolveTimeline(MaterialInstanceDynamic);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MaterialInstanceDynamic=UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance,this);
		Weapon->SetMaterial(0, MaterialInstanceDynamic);
		StartWeaponDissolveTimeline(MaterialInstanceDynamic);
	}
}
