// copyright Paradise_NiseMono

#include "Player/PhosphorPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"

APhosphorPlayerController::APhosphorPlayerController()
{
	bReplicates = true;
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
