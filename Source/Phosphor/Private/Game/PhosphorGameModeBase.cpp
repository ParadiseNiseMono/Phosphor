// copyright Paradise_NiseMono


#include "Game/PhosphorGameModeBase.h"

#include "EngineUtils.h"
#include "Game/LoadScreenSaveGame.h"
#include "Game/PhosphorGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Phosphor/PhosphorLogChannels.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

AActor* APhosphorGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UPhosphorGameInstance* PhosphorGameInstance = Cast<UPhosphorGameInstance>(GetGameInstance());
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == PhosphorGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void APhosphorGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();

	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

ULoadScreenSaveGame* APhosphorGameModeBase::GetSaveSlotData(const int32 SlotIndex, const FString& SlotName)
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void APhosphorGameModeBase::DeleteSlot(const FString& SlotName, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void APhosphorGameModeBase::TravelToMap(UMVVM_LoadSlot* LoadSlot)
{
	const FString LoadSlotName = LoadSlot->GetLoadSlotName();
	const int32	SlotIndex = LoadSlot->SlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, Maps.FindChecked(LoadSlot->GetMapName()));
}

ULoadScreenSaveGame* APhosphorGameModeBase::RetrieveInGameSaveData()
{
	UPhosphorGameInstance* PhosphorGameInstance = Cast<UPhosphorGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = PhosphorGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = PhosphorGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotIndex, InGameLoadSlotName);
}

void APhosphorGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	UPhosphorGameInstance* PhosphorGameInstance = Cast<UPhosphorGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = PhosphorGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = PhosphorGameInstance->LoadSlotIndex;
	PhosphorGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

void APhosphorGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName)
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UPhosphorGameInstance* PhosphorGI = Cast<UPhosphorGameInstance>(GetGameInstance());
	check(PhosphorGI);

	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(PhosphorGI->LoadSlotIndex, PhosphorGI->LoadSlotName))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty();

		for (FActorIterator ActorIt(World); ActorIt; ++ActorIt)
		{
			AActor* Actor = *ActorIt;

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWriter(SavedActor.Bytes);

			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);

			Actor->Serialize(Archive);

			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}

		UGameplayStatics::SaveGameToSlot(SaveGame, PhosphorGI->LoadSlotName, PhosphorGI->LoadSlotIndex);
	}
}

void APhosphorGameModeBase::LoadWorldState(UWorld* World)
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UPhosphorGameInstance* PhosphorGI = Cast<UPhosphorGameInstance>(GetGameInstance());
	check(PhosphorGI);

	if (UGameplayStatics::DoesSaveGameExist(PhosphorGI->LoadSlotName, PhosphorGI->LoadSlotIndex))
	{

		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(PhosphorGI->LoadSlotName, PhosphorGI->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogPhosphor, Warning, TEXT("LoadWorldState: Can't load save game!"));
			return;
		}
		
		for (FActorIterator ActorIt(World); ActorIt; ++ActorIt)
		{
			AActor* Actor = *ActorIt;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					FMemoryReader MemoryReader(SavedActor.Bytes);

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Actor->Serialize(Archive);// converts binary bytes back into variables

					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

FString APhosphorGameModeBase::GetMapNameFromMapAssetName(const FString& MapName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapName)
		{
			return Map.Key;
		}
	}
	return FString();
}

void APhosphorGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap);
}
