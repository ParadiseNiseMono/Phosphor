// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "PhosphorEnemySpawnVolume.generated.h"

class APhosphorEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class PHOSPHOR_API APhosphorEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	APhosphorEnemySpawnVolume();

	/*Save Interface*/
	virtual void LoadActor_Implementation() override;
	/*end Save Interface*/

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
	TArray<APhosphorEnemySpawnPoint*> SpawnPoints;
};
