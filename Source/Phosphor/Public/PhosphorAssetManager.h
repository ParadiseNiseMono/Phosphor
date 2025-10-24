// copyright Paradise_NiseMono

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PhosphorAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PHOSPHOR_API UPhosphorAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UPhosphorAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
