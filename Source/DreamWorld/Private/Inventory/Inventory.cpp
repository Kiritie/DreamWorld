// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Inventory.h"

#include "Inventory/InventoryAgentInterface.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Inventory/Slot/InventoryAuxiliarySlot.h"
#include "Inventory/Slot/InventoryEquipSlot.h"
#include "Inventory/Slot/InventoryGenerateSlot.h"
#include "Inventory/Slot/InventoryShortcutSlot.h"
#include "Inventory/Slot/InventorySkillSlot.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"

UInventory::UInventory()
{
	SplitInfos = TMap<ESplitSlotType, FSplitSlotInfo>();
	Slots = TArray<UInventorySlot*>();
	OwnerActor = nullptr;
	ConnectInventory = nullptr;
	SelectedSlot = nullptr;
}

void UInventory::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	auto SaveData = InSaveData->CastRef<FInventorySaveData>();

	if(bForceMode)
	{
		for(auto Iter : Slots)
		{
			UObjectPoolModuleBPLibrary::DespawnObject(Iter);
		}
		Slots.Empty();
		SplitInfos = SaveData.SplitInfos;
		for (auto Iter : SplitInfos)
		{
			for (int32 i = Iter.Value.StartIndex; i < Iter.Value.StartIndex + Iter.Value.TotalCount; i++)
			{
				UInventorySlot* Slot = nullptr;
				switch(Iter.Key)
				{
					case ESplitSlotType::Default:
					{
						Slot = UObjectPoolModuleBPLibrary::SpawnObject<UInventorySlot>();
						Slot->InitSlot(this, FAbilityItem::Empty, EAbilityItemType::None, Iter.Key);
						break;
					}
					case ESplitSlotType::Shortcut:
					{
						Slot = UObjectPoolModuleBPLibrary::SpawnObject<UInventoryShortcutSlot>();
						Slot->InitSlot(this, FAbilityItem::Empty, EAbilityItemType::None, Iter.Key);
						break;
					}
					case ESplitSlotType::Auxiliary:
					{
						Slot = UObjectPoolModuleBPLibrary::SpawnObject<UInventoryAuxiliarySlot>();
						Slot->InitSlot(this, FAbilityItem::Empty, EAbilityItemType::None, Iter.Key);
						break;
					}
					case ESplitSlotType::Generate:
					{
						Slot = UObjectPoolModuleBPLibrary::SpawnObject<UInventoryGenerateSlot>();
						Slot->InitSlot(this, FAbilityItem::Empty, EAbilityItemType::None, Iter.Key);
						break;
					}
					case ESplitSlotType::Equip:
					{
						Slot = UObjectPoolModuleBPLibrary::SpawnObject<UInventoryEquipSlot>();
						Cast<UInventoryEquipSlot>(Slot)->SetPartType((EDWEquipPartType)(i - Iter.Value.StartIndex));
						Slot->InitSlot(this, FAbilityItem::Empty, EAbilityItemType::Equip, Iter.Key);
						break;
					}
					case ESplitSlotType::Skill:
					{
						Slot = UObjectPoolModuleBPLibrary::SpawnObject<UInventorySkillSlot>();
						Slot->InitSlot(this, FAbilityItem::Empty, EAbilityItemType::Skill, Iter.Key);
						break;
					}
					default: break;
				}
				Slots.Add(Slot);
			}
		}
	}

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->SetItem(SaveData.Items.IsValidIndex(i) ? SaveData.Items[i] : FAbilityItem::Empty);
	}
}

FSaveData* UInventory::ToData()
{
	static FInventorySaveData SaveData;

	SaveData.Reset();
	SaveData.SplitInfos = SplitInfos;

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		SaveData.Items.Add(Slots[i]->GetItem());
	}
	return &SaveData;
}

void UInventory::UnloadData(bool bForceMode)
{
	SplitInfos.Empty();
	for(auto Iter : Slots)
	{
		UObjectPoolModuleBPLibrary::DespawnObject(Iter);
	}
	Slots.Empty();
	OwnerActor = nullptr;
	ConnectInventory = nullptr;
	SelectedSlot = nullptr;
}

void UInventory::Refresh(float DeltaSeconds)
{
	for(int32 i = 0; i < Slots.Num(); i++)
	{
		if(Slots.IsValidIndex(i) && Slots[i])
		{
			Slots[i]->RefreshCooldown(DeltaSeconds);
		}
	}
}

FQueryItemInfo UInventory::GetItemInfoByRange(EQueryItemType InQueryType, FAbilityItem InItem, int32 InStartIndex, int32 InEndIndex)
{
	FQueryItemInfo QueryItemInfo;
	if (InItem.Count <= 0) return QueryItemInfo;
	InStartIndex = FMath::Clamp(InStartIndex, 0, Slots.Num() - 1);
	if(InEndIndex != -1) InEndIndex = FMath::Clamp(InEndIndex, InStartIndex, Slots.Num() - 1);
	QueryItemInfo.Item = InItem;
	switch (InQueryType)
	{
		case EQueryItemType::Addition:
		{
			if (Slots[InStartIndex]->CanPutIn(InItem))
			{
				if (!QueryItemInfo.Slots.Contains(Slots[InStartIndex]))
				{
					QueryItemInfo.Slots.Add(Slots[InStartIndex]);
					InItem.Count -= Slots[InStartIndex]->GetRemainVolume();
					if (InItem.Count <= 0) return QueryItemInfo;
				}
			}
			for (int32 i = (InEndIndex == -1 ? 0 : InStartIndex); i < (InEndIndex == -1 ? Slots.Num() : InEndIndex); i++)
			{
				if (Slots[i]->Contains(InItem) && Slots[i]->CanPutIn(InItem))
				{
					if (!QueryItemInfo.Slots.Contains(Slots[i]))
					{
						QueryItemInfo.Slots.Add(Slots[i]);
						InItem.Count -= Slots[i]->GetRemainVolume();
						if (InItem.Count <= 0) return QueryItemInfo;
					}
				}
			}
			for (int32 i = (InEndIndex == -1 ? 0 : InStartIndex); i < (InEndIndex == -1 ? Slots.Num() : InEndIndex); i++)
			{
				if (Slots[i]->CanPutIn(InItem))
				{
					if (!QueryItemInfo.Slots.Contains(Slots[i]))
					{
						QueryItemInfo.Slots.Add(Slots[i]);
						InItem.Count -= Slots[i]->GetRemainVolume();
						if (InItem.Count <= 0) return QueryItemInfo;
					}
				}
			}
			break;
		}
		case EQueryItemType::Remove:
		{
			if (Slots[InStartIndex]->Contains(InItem))
			{
				if (!QueryItemInfo.Slots.Contains(Slots[InStartIndex]))
				{
					QueryItemInfo.Slots.Add(Slots[InStartIndex]);
					InItem.Count -= Slots[InStartIndex]->GetItem().Count;
					if (InItem.Count <= 0) return QueryItemInfo;
				}
			}
			for (int32 i = (InEndIndex == -1 ? 0 : InStartIndex); i < (InEndIndex == -1 ? Slots.Num() : InEndIndex); i++)
			{
				if (Slots[i]->Contains(InItem))
				{
					if (!QueryItemInfo.Slots.Contains(Slots[i]))
					{
						QueryItemInfo.Slots.Add(Slots[i]);
						InItem.Count -= Slots[i]->GetItem().Count;
						if (InItem.Count <= 0) return QueryItemInfo;
					}
				}
			}
			break;
		}
		case EQueryItemType::Clear:
		{
			for (int32 i = (InEndIndex == -1 ? 0 : InStartIndex); i < (InEndIndex == -1 ? Slots.Num() : InEndIndex); i++)
			{
				if (Slots[i]->Contains(InItem))
				{
					if (!QueryItemInfo.Slots.Contains(Slots[i]))
						QueryItemInfo.Slots.Add(Slots[i]);
				}
			}
			break;
		}
	}
	QueryItemInfo.Item -= InItem;
	return QueryItemInfo;
}

FQueryItemInfo UInventory::GetItemInfoBySplitType(EQueryItemType InQueryType, FAbilityItem InItem, ESplitSlotType InSplitSlotType)
{
	const FSplitSlotInfo SplitSlotInfo = GetSplitSlotInfo(InSplitSlotType);
	return GetItemInfoByRange(InQueryType, InItem, SplitSlotInfo.StartIndex, SplitSlotInfo.StartIndex + SplitSlotInfo.TotalCount);
}

void UInventory::AdditionItem(FAbilityItem& InItem)
{
	AdditionItemByRange(InItem);
}

void UInventory::AdditionItemBySlots(FAbilityItem& InItem, const TArray<UInventorySlot*>& InSlots)
{
	for (int i = 0; i < InSlots.Num(); i++)
	{
		InSlots[i]->AddItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::AdditionItemByRange(FAbilityItem& InItem, int32 InStartIndex, int32 InEndIndex)
{
	auto QueryItemInfo = GetItemInfoByRange(EQueryItemType::Addition, InItem, InStartIndex, InEndIndex);
	for (int i = 0; i < QueryItemInfo.Slots.Num(); i++)
	{
		QueryItemInfo.Slots[i]->AddItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::AdditionItemBySplitType(FAbilityItem& InItem, ESplitSlotType InSplitSlotType)
{
	auto QueryItemInfo = GetItemInfoBySplitType(EQueryItemType::Addition, InItem, InSplitSlotType);
	for (int i = 0; i < QueryItemInfo.Slots.Num(); i++)
	{
		QueryItemInfo.Slots[i]->AddItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::RemoveItemBySlots(FAbilityItem& InItem, const TArray<UInventorySlot*>& InSlots)
{
	for (int i = 0; i < InSlots.Num(); i++)
	{
		InSlots[i]->SubItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::RemoveItemByRange(FAbilityItem& InItem, int32 InStartIndex, int32 InEndIndex)
{
	auto QueryItemInfo = GetItemInfoByRange(EQueryItemType::Remove, InItem, InStartIndex, InEndIndex);
	for (int i = 0; i < QueryItemInfo.Slots.Num(); i++)
	{
		QueryItemInfo.Slots[i]->SubItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::RemoveItemBySplitType(FAbilityItem& InItem, ESplitSlotType InSplitSlotType)
{
	auto QueryItemInfo = GetItemInfoBySplitType(EQueryItemType::Remove, InItem, InSplitSlotType);
	for (int i = 0; i < QueryItemInfo.Slots.Num(); i++)
	{
		QueryItemInfo.Slots[i]->SubItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::MoveItemByRange(UInventory* InTargetInventory, FAbilityItem& InItem, int32 InSelfStartIndex, int32 InSelfEndIndex, int32 InTargetStartIndex, int32 InTargetEndIndex)
{
	const auto QueryItemInfo = InTargetInventory->GetItemInfoByRange(EQueryItemType::Remove, InItem, InTargetStartIndex, InTargetEndIndex);
	InItem.Count = InItem.Count != -1 ? FMath::Min(InItem.Count, QueryItemInfo.Item.Count) : QueryItemInfo.Item.Count;
	FAbilityItem tmpItem = FAbilityItem(InItem);
	RemoveItemByRange(tmpItem, InSelfStartIndex, InSelfEndIndex);
	InItem -= tmpItem;
	InTargetInventory->AdditionItemByRange(InItem, InTargetStartIndex, InTargetEndIndex);
}

void UInventory::MoveItemBySplitType(UInventory* InTargetInventory, FAbilityItem& InItem, ESplitSlotType InSelfSplitSlotType, ESplitSlotType InTargetSplitSlotType)
{
	const auto QueryItemInfo = InTargetInventory->GetItemInfoBySplitType(EQueryItemType::Remove, InItem, InTargetSplitSlotType);
	InItem.Count = InItem.Count != -1 ? FMath::Min(InItem.Count, QueryItemInfo.Item.Count) : QueryItemInfo.Item.Count;
	FAbilityItem tmpItem = FAbilityItem(InItem);
	RemoveItemBySplitType(tmpItem, InSelfSplitSlotType);
	InItem -= tmpItem;
	InTargetInventory->AdditionItemBySplitType(InItem, InTargetSplitSlotType);
}

void UInventory::ClearItem(FAbilityItem& InItem)
{
	auto QueryItemInfo = GetItemInfoByRange(EQueryItemType::Clear, InItem);
	for (int i = 0; i < QueryItemInfo.Slots.Num(); i++)
	{
		QueryItemInfo.Slots[i]->ClearItem();
	}
}

void UInventory::ClearAllItem()
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->ClearItem();
	}
}

void UInventory::DiscardAllItem()
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->DiscardItem();
	}
}

void UInventory::SetSelectedSlot(UInventorySlot* InSelectedSlot)
{
	SelectedSlot = InSelectedSlot;
	if(IInventoryAgentInterface* InventoryAgent = Cast<IInventoryAgentInterface>(GetOwnerActor()))
	{
		InventoryAgent->OnInventorySlotSelected(InSelectedSlot);
	}
	OnSlotSelected.Broadcast(InSelectedSlot);
}

FAbilityItem& UInventory::GetSelectedItem() const
{
	if(GetSelectedSlot())
	{
		return GetSelectedSlot()->GetItem();
	}
	return FAbilityItem::Empty;
}

int32 UInventory::GetSlotsNum() const
{
	return Slots.Num();
}

bool UInventory::HasSplitSlotInfo(ESplitSlotType InSplitSlotType) const
{
	return SplitInfos.Contains(InSplitSlotType);
}

FSplitSlotInfo UInventory::GetSplitSlotInfo(ESplitSlotType InSplitSlotType) const
{
	if(InSplitSlotType != ESplitSlotType::None)
	{
		if(HasSplitSlotInfo(InSplitSlotType))
		{
			return SplitInfos[InSplitSlotType];
		}
	}
	else
	{
		return FSplitSlotInfo(0, GetSlotsNum() - 1);
	}
	return FSplitSlotInfo();
}

TArray<UInventorySlot*> UInventory::GetSplitSlots(ESplitSlotType InSplitSlotType)
{
	TArray<UInventorySlot*> SplitSlots = TArray<UInventorySlot*>();

	for (auto Iter : SplitInfos)
	{
		if (Iter.Key == InSplitSlotType)
		{
			for (int32 i = Iter.Value.StartIndex; i < Iter.Value.StartIndex + Iter.Value.TotalCount; i++)
			{
				SplitSlots.Add(Slots[i]);
			}
			break;
		}
	}

	return SplitSlots;
}

TMap<ESplitSlotType, FSplitSlotData> UInventory::GetSplitSlotDatas()
{
	TMap<ESplitSlotType, FSplitSlotData> SplitSlotDatas = TMap<ESplitSlotType, FSplitSlotData>();

	for (auto Iter : SplitInfos)
	{
		FSplitSlotData SplitSlotData = FSplitSlotData();
		for (int32 i = Iter.Value.StartIndex; i < Iter.Value.StartIndex + Iter.Value.TotalCount; i++)
		{
			SplitSlotData.Slots.Add(Slots[i]);
		}
		SplitSlotDatas.Add(Iter.Key, SplitSlotData);
	}

	return SplitSlotDatas;
}
