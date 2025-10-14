// copyright Paradise_NiseMono

#include "Player/PhosphorPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

APhosphorPlayerController::APhosphorPlayerController()
{
	bReplicates = true;
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
	check(Subsystem);
	Subsystem->AddMappingContext(PhosphorContext, 0);

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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,
		this,&APhosphorPlayerController::move);
}

void APhosphorPlayerController::move(const FInputActionValue& InputActionValue)
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
			ThisActor->HightLightActor();
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
			LastActor->UnHightLightActor();
		}
		else
		{
			//both actors are valid
			if (LastActor!=ThisActor)
			{
				//case D
				LastActor->UnHightLightActor();
				ThisActor->HightLightActor();
			}
			else
			{
				//case E
			}
		}
	}
}
