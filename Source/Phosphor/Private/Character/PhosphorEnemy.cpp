// copyright Paradise_NiseMono


#include "Character/PhosphorEnemy.h"

void APhosphorEnemy::HightLightActor()
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Highlighted Actor: %s"), *GetName()));
	bHightLighted = true;
}

void APhosphorEnemy::UnHightLightActor()
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("UnHighlighted Actor: %s"), *GetName()));
	bHightLighted = false;
}
