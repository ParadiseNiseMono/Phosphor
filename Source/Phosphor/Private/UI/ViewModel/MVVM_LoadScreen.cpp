// copyright Paradise_NiseMono


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/PhosphorGameInstance.h"
#include "Game/PhosphorGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName("LoadSlot_1");
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName("LoadSlot_2");
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	SetNumLoadSlots(LoadSlots.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	APhosphorGameModeBase* PhosphorGameMode = Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(PhosphorGameMode))
	{
		GEngine->AddOnScreenDebugMessage(Slot, 5.0f, FColor::Red, FString("Please switch to single play"));
		return;
	}

	LoadSlots[Slot]->SetMapName(PhosphorGameMode->DefaultMapName);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SetPlayerLevel(1);
	LoadSlots[Slot]->PlayerStartTag = PhosphorGameMode->DefaultPlayerStartTag;
	LoadSlots[Slot]->MapAssetName = PhosphorGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
	
	PhosphorGameMode->SaveSlotData(LoadSlots[Slot], Slot);

	LoadSlots[Slot]->InitializeSlot();

	UPhosphorGameInstance* PhosphorGameInstance = Cast<UPhosphorGameInstance>(PhosphorGameMode->GetGameInstance());
	PhosphorGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	PhosphorGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	PhosphorGameInstance->PlayerStartTag = PhosphorGameMode->DefaultPlayerStartTag;
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (const auto& LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		APhosphorGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	APhosphorGameModeBase* PhosphorGameMode = Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(this));

	UPhosphorGameInstance* PhosphorGameInstance = Cast<UPhosphorGameInstance>(PhosphorGameMode->GetGameInstance());
	PhosphorGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	PhosphorGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	PhosphorGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	
	if (IsValid(SelectedSlot))
	{
		PhosphorGameMode->TravelToMap(SelectedSlot);
	}
}

void UMVVM_LoadScreen::LoadData()
{
	APhosphorGameModeBase* PhosphorGameMode = Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(PhosphorGameMode)) return;
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveGameObject = PhosphorGameMode->GetSaveSlotData(LoadSlot.Key, LoadSlot.Value->GetLoadSlotName());

		LoadSlot.Value->SetPlayerName(SaveGameObject->PlayerName);
		LoadSlot.Value->SlotStatus = SaveGameObject->SaveSlotStatus;
		LoadSlot.Value->SetMapName(SaveGameObject->MapName);
		LoadSlot.Value->PlayerStartTag = SaveGameObject->PlayerStartTag;
		LoadSlot.Value->SetPlayerLevel(SaveGameObject->PlayerLevel);
		LoadSlot.Value->InitializeSlot();
	}
}

void UMVVM_LoadScreen::SetNumLoadSlots(const int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
