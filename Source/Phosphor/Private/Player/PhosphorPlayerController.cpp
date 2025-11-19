// copyright Paradise_NiseMono

#include "Player/PhosphorPlayerController.h"
#include <EnhancedInputSubsystems.h>

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystem/PhosphorAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"

#include "Input/PhosphorInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Phosphor/Phosphor.h"
#include "UI/Widget/DamageTextComponent.h"

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
	UpdateMagicCircleLocation();
}

void APhosphorPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial != nullptr)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void APhosphorPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void APhosphorPlayerController::ShowDamageNumber_Implementation(const float Damage,ACharacter* Target,bool bBlockHit,bool bCriticalHit)
{
	if (IsValid(Target) && DamageTextComponentClass&&IsLocalController())
	{
		UDamageTextComponent* DamageText=NewObject<UDamageTextComponent>(Target,DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(Target->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(Damage,bBlockHit,bCriticalHit);
	}
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
	PhosphorInputComponent->BindAction(ShiftAction,ETriggerEvent::Started,this,&APhosphorPlayerController::ShiftKeyPressed);
	PhosphorInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&APhosphorPlayerController::ShiftKeyReleased);
	PhosphorInputComponent->BindAbilityInputAction(
		PhosphorInputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
}

void APhosphorPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC() -> HasMatchingGameplayTag(FPhosphorGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputAxisValue=InputActionValue.Get<FVector2D>();
	FRotator Rotator=GetControlRotation();
	FRotator YawRotator=FRotator(0.f,Rotator.Yaw,0.f);

	const FVector ForwardVector=FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightVector=FRotationMatrix(Rotator).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn=GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardVector,InputAxisValue.Y);
		ControlledPawn->AddMovementInput(RightVector,InputAxisValue.X);
		bAutoRunning=false;
	}
}

void APhosphorPlayerController::CursorTrace()
{

	if (GetASC() && GetASC() -> HasMatchingGameplayTag(FPhosphorGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor) LastActor->UnHighLightActor();
		if (ThisActor) ThisActor->UnHighLightActor();
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}

	const ECollisionChannel TraceChannel = IsValid(MagicCircle)? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel,false,CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor=ThisActor;
	ThisActor=CursorHit.GetActor();
	
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
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}
		}
	}
}

void APhosphorPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC() -> HasMatchingGameplayTag(FPhosphorGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (InputTag.MatchesTagExact(FPhosphorGameplayTags::Get().InputTag_LMB))
	{
		bTargeting=ThisActor ? true : false;
		bAutoRunning=false;
	}
	if(GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void APhosphorPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC() -> HasMatchingGameplayTag(FPhosphorGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(FPhosphorGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}
	if (GetASC())GetASC()->AbilityInputTagReleased(InputTag);
	if (!bTargeting&&!bShiftKeyPressed)
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
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
		}
		FollowTime=0.0f;
		bTargeting=false;
	}
}

void APhosphorPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC() -> HasMatchingGameplayTag(FPhosphorGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(FPhosphorGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	if (bTargeting||bShiftKeyPressed)
	{
		if (GetASC())GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime+=GetWorld()->GetDeltaSeconds();
		if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,CursorHit))CachedDestination=CursorHit.ImpactPoint;
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

void APhosphorPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}
