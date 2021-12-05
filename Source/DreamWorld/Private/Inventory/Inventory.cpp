// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Inventory.h"

#include "Inventory/Slot/InventorySlot.h"
#include "Vitality/Vitality.h"
#include "Abilities/Item/DWItemAbility.h"
#include "Inventory/Slot/InventoryAuxiliarySlot.h"
#include "Inventory/Slot/InventoryEquipSlot.h"
#include "Inventory/Slot/InventoryGenerateSlot.h"
#include "Inventory/Slot/InventoryShortcutSlot.h"
#include "Inventory/Slot/InventorySkillSlot.h"

UInventory::UInventory()
{
	OwnerActor = nullptr;
	ConnectInventory = nullptr;
	SelectedSlot = nullptr;
	Slots = TArray<UInventorySlot*>();
}

void UInventory::Initialize(AActor* InOwner, TMap<ESplitSlotType, FSplitSlotInfo> InSplitInfos)
{
	for (auto Iter : Slots)
	{
		Iter->ConditionalBeginDestroy();
	}
	Slots.Empty();
	
	OwnerActor = InOwner;
	SplitInfos = InSplitInfos;
	for (auto Iter : SplitInfos)
	{
		for (int32 i = Iter.Value.StartIndex; i < Iter.Value.StartIndex + Iter.Value.TotalCount; i++)
		{
			UInventorySlot* Slot = nullptr;
			switch(Iter.Key)
			{
				case ESplitSlotType::Default:
				{
					Slot = NewObject<UInventorySlot>(OwnerActor);
					Slot->InitSlot(this, FItem::Empty, EItemType::None, Iter.Key);
					break;
				}
				case ESplitSlotType::Shortcut:
				{
					Slot = NewObject<UInventoryShortcutSlot>(OwnerActor);
					Slot->InitSlot(this, FItem::Empty, EItemType::None, Iter.Key);
					break;
				}
				case ESplitSlotType::Auxiliary:
				{
					Slot = NewObject<UInventoryAuxiliarySlot>(OwnerActor);
					Slot->InitSlot(this, FItem::Empty, EItemType::None, Iter.Key);
					break;
				}
				case ESplitSlotType::Generate:
				{
					Slot = NewObject<UInventoryGenerateSlot>(OwnerActor);
					Slot->InitSlot(this, FItem::Empty, EItemType::None, Iter.Key);
					break;
				}
				case ESplitSlotType::Equip:
				{
					Slot = NewObject<UInventoryEquipSlot>(OwnerActor);
					Cast<UInventoryEquipSlot>(Slot)->SetPartType((EEquipPartType)(i - Iter.Value.StartIndex));
					Slot->InitSlot(this, FItem::Empty, EItemType::Equip, Iter.Key);
					break;
				}
				case ESplitSlotType::Skill:
				{
					Slot = NewObject<UInventorySkillSlot>(OwnerActor);
					Slot->InitSlot(this, FItem::Empty, EItemType::Skill, Iter.Key);
					break;
				}
			}
			Slots.Add(Slot);
		}
	}
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

void UInventory::LoadData(FInventorySaveData InInventoryData, AActor* InOwner)
{
	Initialize(InOwner, InInventoryData.SplitInfos);
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		Slots[i]->SetItem(InInventoryData.Items.IsValidIndex(i) ? InInventoryData.Items[i] : FItem::Empty);
	}
}

FInventorySaveData UInventory::ToData(bool bSaved)
{
	auto data = FInventorySaveData();

	data.bSaved = bSaved;
	data.SplitInfos = SplitInfos;

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		data.Items.Add(Slots[i]->GetItem());
	}
	return data;
}

FQueryItemInfo UInventory::GetItemInfoByRange(EQueryItemType InQueryType, FItem InItem, int32 InStartIndex, int32 InEndIndex)
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

FQueryItemInfo UInventory::GetItemInfoBySplitType(EQueryItemType InQueryType, FItem InItem, ESplitSlotType InSplitSlotType)
{
	const FSplitSlotInfo SplitSlotInfo = GetSplitSlotInfo(InSplitSlotType);
	return GetItemInfoByRange(InQueryType, InItem, SplitSlotInfo.StartIndex, SplitSlotInfo.StartIndex + SplitSlotInfo.TotalCount);
}

void UInventory::AdditionItemByRange(FItem& InItem, int32 InStartIndex, int32 InEndIndex)
{
	auto QueryItemInfo = GetItemInfoByRange(EQueryItemType::Addition, InItem, InStartIndex, InEndIndex);
	for (int i = 0; i < QueryItemInfo.Slots.Num(); i++)
	{
		QueryItemInfo.Slots[i]->AddItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::AdditionItemBySplitType(FItem& InItem, ESplitSlotType InSplitSlotType)
{
	auto QueryItemInfo = GetItemInfoBySplitType(EQueryItemType::Addition, InItem, InSplitSlotType);
	for (int i = 0; i < QueryItemInfo.Slots.Num(); i++)
	{
		QueryItemInfo.Slots[i]->AddItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::RemoveItemByRange(FItem& InItem, int32 InStartIndex, int32 InEndIndex)
{
	auto QueryItemInfo = GetItemInfoByRange(EQueryItemType::Remove, InItem, InStartIndex, InEndIndex);
	for (int i = 0; i < QueryItemInfo.Slots.Num(); i++)
	{
		QueryItemInfo.Slots[i]->SubItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::RemoveItemBySplitType(FItem& InItem, ESplitSlotType InSplitSlotType)
{
	auto QueryItemInfo = GetItemInfoBySplitType(EQueryItemType::Remove, InItem, InSplitSlotType);
	for (int i = 0; i < QueryItemInfo.Slots.Num(); i++)
	{
		QueryItemInfo.Slots[i]->SubItem(InItem);
		if (InItem.Count <= 0) break;
	}
}

void UInventory::MoveItemByRange(UInventory* InTargetInventory, FItem& InItem, int32 InSelfStartIndex, int32 InSelfEndIndex, int32 InTargetStartIndex, int32 InTargetEndIndex)
{
	const auto QueryItemInfo = InTargetInventory->GetItemInfoByRange(EQueryItemType::Remove, InItem, InTargetStartIndex, InTargetEndIndex);
	InItem.Count = InItem.Count != -1 ? FMath::Min(InItem.Count, QueryItemInfo.Item.Count) : QueryItemInfo.Item.Count;
	FItem tmpItem = FItem(InItem);
	RemoveItemByRange(tmpItem, InSelfStartIndex, InSelfEndIndex);
	InItem -= tmpItem;
	InTargetInventory->AdditionItemByRange(InItem, InTargetStartIndex, InTargetEndIndex);
}

void UInventory::MoveItemBySplitType(UInventory* InTargetInventory, FItem& InItem, ESplitSlotType InSelfSplitSlotType, ESplitSlotType InTargetSplitSlotType)
{
	const auto QueryItemInfo = InTargetInventory->GetItemInfoBySplitType(EQueryItemType::Remove, InItem, InTargetSplitSlotType);
	InItem.Count = InItem.Count != -1 ? FMath::Min(InItem.Count, QueryItemInfo.Item.Count) : QueryItemInfo.Item.Count;
	FItem tmpItem = FItem(InItem);
	RemoveItemBySplitType(tmpItem, InSelfSplitSlotType);
	InItem -= tmpItem;
	InTargetInventory->AdditionItemBySplitType(InItem, InTargetSplitSlotType);
}

void UInventory::ClearItem(FItem& InItem)
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

FItem& UInventory::GetSelectedItem() const
{
	if(GetSelectedSlot())
	{
		return GetSelectedSlot()->GetItem();
	}
	return FItem::Empty;
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

TArray<UInventorySlot*> UInventory::K2_GetSplitSlots(ESplitSlotType InSplitSlotType)
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
