// copyright Paradise_NiseMono


#include "Character/PhosphorEnemy.h"

#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Phosphor/Phosphor.h"
#include "UI/Widget/PhosphorUserWidget.h"

APhosphorEnemy::APhosphorEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent=CreateDefaultSubobject<UPhosphorAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet=CreateDefaultSubobject<UPhosphorAttributeSet>("AttributeSet");

	HealthBar=CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void APhosphorEnemy::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void APhosphorEnemy::UnHighLightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void APhosphorEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();


	if (UPhosphorUserWidget* PhosphorWidget=Cast<UPhosphorUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		PhosphorWidget->SetWidgetController(this);
	}
	if (const UPhosphorAttributeSet* PhosphorAttributeSet=Cast<UPhosphorAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PhosphorAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PhosphorAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		OnHealthChanged.Broadcast(PhosphorAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(PhosphorAttributeSet->GetMaxHealth());
	}
}

void APhosphorEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UPhosphorAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttribute();
}

void APhosphorEnemy::InitializeDefaultAttribute() const
{
	UPhosphorAbilitySystemLibrary::InitializeDefaultAbilities(this,CharacterClass,Level,AbilitySystemComponent);
}
