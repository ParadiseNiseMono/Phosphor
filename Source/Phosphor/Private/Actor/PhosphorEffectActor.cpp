// copyright Paradise_NiseMono


#include "Actor/PhosphorEffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "Components/SphereComponent.h"

APhosphorEffectActor::APhosphorEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMesh);
	
	SphereComponent=CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetupAttachment(StaticMesh);
}

void APhosphorEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APhosphorEffectActor::OnOverLap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APhosphorEffectActor::EndOverLap);
}

void APhosphorEffectActor::OnOverLap(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	//TODO: Change this to apply a Gameplay Effect.For now, using const_cast to hack!
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UPhosphorAttributeSet* PhosphorAttributeSet=Cast<UPhosphorAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UPhosphorAttributeSet::StaticClass()));
		UPhosphorAttributeSet* MutablePhosphorAttributeSet=const_cast<UPhosphorAttributeSet*>(PhosphorAttributeSet);
		MutablePhosphorAttributeSet->SetHealth(PhosphorAttributeSet->GetHealth() +25.0f);
		MutablePhosphorAttributeSet->SetMana(PhosphorAttributeSet->GetMana() -15.0f);
		Destroy();
	}
	
}

void APhosphorEffectActor::EndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


