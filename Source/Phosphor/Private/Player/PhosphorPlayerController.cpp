// copyright Paradise_NiseMono

#include "Player/PhosphorPlayerController.h"
#include <EnhancedInputSubsystems.h>

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "Components/SplineComponent.h"

#include "Input/PhosphorInputComponent.h"
#include "Interaction/EnemyInterface.h"

APhosphorPlayerController::APhosphorPlayerController()
{
	bReplicates = true;
	Spline=CreateDefaultSubobject<USplineComponent>("Spline");
}

void APhosphorPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void APhosphorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(PhosphorContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(PhosphorContext, 0);
	}


	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void APhosphorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UPhosphorInputComponent* PhosphorInputComponent = CastChecked<UPhosphorInputComponent>(InputComponent);
	PhosphorInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&APhosphorPlayerController::Move);

	PhosphorInputComponent->BindAbilityInputAction(
		PhosphorInputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
}

void APhosphorPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisValue=InputActionValue.Get<FVector2D>();
	FRotator Rotator=GetControlRotation();
	FRotator YawRotator=FRotator(0.f,Rotator.Yaw,0.f);

	const FVector ForwardVector=FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightVector=FRotationMatrix(Rotator).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn=GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardVector,InputAxisValue.Y);
		ControlledPawn->AddMovementInput(RightVector,InputAxisValue.X);
	}
}

void APhosphorPlayerController::CursorTrace()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility,false,HitResult);
	if (!HitResult.bBlockingHit) return;

	LastActor=ThisActor;
	ThisActor=HitResult.GetActor();

	/*
	 *Line trace from cursor.Here has several scenarios
	 *A.LastActor null and ThisActor null
	 *Do nothing
	 * B. LastActor null and ThisActor valid
	 * HightLight ThisActor
	 * C. LastActor valid and ThisActor null
	 * UnHightLight LastActor
	 * D.Both actors are valid,but ThisActor!=LastActor
	 * UnHightLight LastActor,and HightLight ThisActor
	 * E. Both actors are valid,and are same actor
	 * do nothing
	 */

	if (LastActor==nullptr)
	{
		if (ThisActor!=nullptr)
		{
			//case B
			ThisActor->HighLightActor();
		}
		else
		{
			//case A
		}
	}else if(LastActor!=nullptr)
	{
		if (ThisActor==nullptr)
		{
			//case C
			LastActor->UnHighLightActor();
		}
		else
		{
			//both actors are valid
			if (LastActor!=ThisActor)
			{
				//case D
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}
			else
			{
				//case E
			}
		}
	}
}

void APhosphorPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FPhosphorGameplayTags::Get().InputTag_LMB))
	{
		bTargeting=ThisActor ? true : false;
		bAutoRunning=false;
	}
}

void APhosphorPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC()==nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void APhosphorPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FPhosphorGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime+=GetWorld()->GetDeltaSeconds();
		FHitResult HitResult;
		if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult))
		{
			CachedDestination=HitResult.ImpactPoint;
		}
		if (APawn* ControlledPawn=GetPawn())
		{
			const FVector WorldDestination=(CachedDestination-ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDestination);
		}
	}
}

UPhosphorAbilitySystemComponent* APhosphorPlayerController::GetASC()
{
	if (PhosphorAbilitySystemComponent==nullptr)
	{
		 PhosphorAbilitySystemComponent = Cast<UPhosphorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return PhosphorAbilitySystemComponent;
}
