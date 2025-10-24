// copyright Paradise_NiseMono


#include "PhosphorAssetManager.h"
#include "PhosphorGameplayTags.h"
#include "AbilitySystemGlobals.h"

UPhosphorAssetManager& UPhosphorAssetManager::Get()
{
	check(GEngine);

	UPhosphorAssetManager* PhosphorAssetManager=Cast<UPhosphorAssetManager>(GEngine->AssetManager);
	return *PhosphorAssetManager;
}

void UPhosphorAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FPhosphorGameplayTags::InitializeNativeGameplayTags();


	//This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}
