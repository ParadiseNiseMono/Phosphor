// copyright Paradise_NiseMono

#include "Player/PhosphorPlayerController.h"
#include <EnhancedInputSubsystems.h>

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
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
	AutoRun();
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
	GetHitResultUnderCursor(ECC_Visibility,false,HitResult);
	if (!HitResult.bBlockingHit) return;

	LastActor=ThisActor;
	ThisActor=HitResult.GetActor();
	
	if (LastActor==nullptr)
	{
		if (ThisActor!=nullptr)ThisActor->HighLightActor();
	}else if(LastActor!=nullptr)
	{
		if (ThisActor==nullptr)LastActor->UnHighLightActor();
		else
		{
			if (LastActor!=ThisActor)
			{
				//case D
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
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
	if (!InputTag.MatchesTagExact(FPhosphorGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}
	if (bTargeting)
	{
		if (GetASC())GetASC()->AbilityInputTagReleased(InputTag);
	}
	else
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime<=ShortPressThreshold&&ControlledPawn)
		{
			if ( UNavigationPath* NavigationPath= UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLocation:NavigationPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLocation,ESplineCoordinateSpace::World);
				}
				if (!NavigationPath->PathPoints.IsEmpty())
				{
					CachedDestination=NavigationPath->PathPoints.Last();
					bAutoRunning=true;
				}
			}
		}
		FollowTime=0.0f;
		bTargeting=false;
	}
}

void APhosphorPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FPhosphorGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	if (bTargeting)
	{
		if (GetASC())GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime+=GetWorld()->GetDeltaSeconds();
		if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult))CachedDestination=HitResult.ImpactPoint;
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

void APhosphorPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn= GetPawn())
	{
		const FVector LocationOnSpline=Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector Direction=Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanToDestination=(LocationOnSpline-CachedDestination).Length();
		if (DistanToDestination<=AutoRunAcceptanceRadius)bAutoRunning=false;
	}
}
