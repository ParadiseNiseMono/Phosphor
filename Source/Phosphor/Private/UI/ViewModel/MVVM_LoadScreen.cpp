// copyright Paradise_NiseMono


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/PhosphorGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName("LoadSlot_0");
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName("LoadSlot_1");
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName("LoadSlot_2");
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

	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	PhosphorGameMode->SaveSlotData(LoadSlots[Slot], Slot);

	LoadSlots[Slot]->InitializeSlot();
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
}

void UMVVM_LoadScreen::LoadData()
{
	APhosphorGameModeBase* PhosphorGameMode = Cast<APhosphorGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveGameObject = PhosphorGameMode->GetSaveSlotData(LoadSlot.Key, LoadSlot.Value->GetLoadSlotName());

		LoadSlot.Value->SetPlayerName(SaveGameObject->PlayerName);
		LoadSlot.Value->SlotStatus = SaveGameObject->SaveSlotStatus;
		LoadSlot.Value->InitializeSlot();
	}
}

void UMVVM_LoadScreen::SetNumLoadSlots(const int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
