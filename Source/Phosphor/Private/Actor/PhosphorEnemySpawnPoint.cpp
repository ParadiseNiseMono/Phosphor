// copyright Paradise_NiseMono


#include "Actor/PhosphorEnemySpawnPoint.h"

#include "Character/PhosphorEnemy.h"

void APhosphorEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APhosphorEnemy* Enemy = GetWorld()->SpawnActorDeferred<APhosphorEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
