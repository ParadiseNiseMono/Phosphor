// copyright Paradise_NiseMono


#include "Character/PhosphorEnemy.h"

#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "AbilitySystem/PhosphorAbilitySystemLibrary.h"
#include "AbilitySystem/PhosphorAttributeSet.h"
#include "AI/PhosphorAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Phosphor/Phosphor.h"
#include "UI/Widget/PhosphorUserWidget.h"

APhosphorEnemy::APhosphorEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent=CreateDefaultSubobject<UPhosphorAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet=CreateDefaultSubobject<UPhosphorAttributeSet>("AttributeSet");

	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;
	GetCharacterMovement()->bOrientRotationToMovement=true;
	
	HealthBar=CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void APhosphorEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	PhosphorAIController=Cast<APhosphorAIController>(NewController);

	PhosphorAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	PhosphorAIController->RunBehaviorTree(BehaviorTree);
	PhosphorAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),false);
	PhosphorAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),CharacterClass != ECharacterClass::Warrior);
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

void APhosphorEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void APhosphorEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget=InCombatTarget;
}

AActor* APhosphorEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void APhosphorEnemy::HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	bHitReacting=NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed=bHitReacting ? 0.0f : BaseWalkSpeed;
	if (PhosphorAIController&&PhosphorAIController->GetBlackboardComponent())
	{
		PhosphorAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),bHitReacting);
	}
}

void APhosphorEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed=BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UPhosphorAbilitySystemLibrary::GiveStartUpAbilities(this,AbilitySystemComponent,CharacterClass);	
	}

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
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FPhosphorGameplayTags::Get().Effects_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&APhosphorEnemy::HitReactTagChanged
		);
		
		OnHealthChanged.Broadcast(PhosphorAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(PhosphorAttributeSet->GetMaxHealth());
	}
}

void APhosphorEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UPhosphorAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttribute();
	}
}

void APhosphorEnemy::InitializeDefaultAttribute() const
{
	UPhosphorAbilitySystemLibrary::InitializeDefaultAbilities(this,CharacterClass,Level,AbilitySystemComponent);
}
