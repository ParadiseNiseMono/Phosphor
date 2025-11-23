// copyright Paradise_NiseMono


#include "Checkpoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/PhosphorGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComponent);
}

void AMapEntrance::HighLightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::LoadActor_Implementation()
{
	
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (APhosphorGameModeBase* PhosphorGM = Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			//Save the world that we are traveling to.
			PhosphorGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());	
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
