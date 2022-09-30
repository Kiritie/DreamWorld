// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/InventorySlot.h"

#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Abilities/ItemAbilityBase.h"
#include "Ability/Components/AbilitySystemComponentBase.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Inventory/Inventory.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Global/GlobalBPLibrary.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetItemInfoBox.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"

UInventorySlot::UInventorySlot()
{
	Item = FAbilityItem::Empty;
	Inventory = nullptr;
	LimitType = EAbilityItemType::None;
	SplitType = ESplitSlotType::Default;
}

void UInventorySlot::OnInitialize(UInventory* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType /*= EAbilityItemType::None*/, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Item = InItem;
	Inventory = InInventory;
	LimitType = InLimitType;
	SplitType = InSplitType;
}

void UInventorySlot::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	
}

void UInventorySlot::OnDespawn_Implementation()
{
	SetItem(FAbilityItem::Empty);
	Inventory = nullptr;
	LimitType = EAbilityItemType::None;
	SplitType = ESplitSlotType::Default;
}

bool UInventorySlot::CheckSlot(FAbilityItem& InItem) const
{
	return LimitType == EAbilityItemType::None || InItem.GetData().GetItemType() == LimitType;
}

bool UInventorySlot::CanPutIn(FAbilityItem& InItem) const
{
	return (IsEmpty() && CheckSlot(InItem)) || (Item.EqualType(InItem) && GetRemainVolume(InItem) > 0);
}

bool UInventorySlot::IsMatch(FAbilityItem InItem, bool bForce) const
{
	if(!InItem.IsValid()) return false;

	const auto ItemType = InItem.GetData().GetItemType();
	switch(SplitType)
	{
		case ESplitSlotType::Default:
		case ESplitSlotType::Shortcut:
		case ESplitSlotType::Auxiliary:
		{
			if(!bForce)
			{
				return ItemType != EAbilityItemType::Skill && ItemType != EAbilityItemType::Equip;
			}
			break;
		}
		case ESplitSlotType::Skill:
		{
			return ItemType == EAbilityItemType::Skill;
		}
		case ESplitSlotType::Equip:
		{
			return ItemType == EAbilityItemType::Equip;
		}
		default: break;
	}
	return false;
}

bool UInventorySlot::Contains(FAbilityItem& InItem) const
{
	return !IsEmpty() && Item.EqualType(InItem);
}

void UInventorySlot::Refresh()
{
	if(Item.IsValid() && Item.Count <= 0)
	{
		SetItem(FAbilityItem::Empty, false);
	}
	OnInventorySlotRefresh.Broadcast();
}

void UInventorySlot::OnItemPreChange(FAbilityItem& InNewItem)
{
	
}

void UInventorySlot::OnItemChanged(FAbilityItem& InOldItem)
{
	if(Item.IsValid())
	{
		IAbilityVitalityInterface* Vitality = Cast<IAbilityVitalityInterface>(Inventory->GetOwnerActor());
		if (Vitality && Item.GetData().AbilityClass)
		{
			Item.AbilityHandle = Vitality->GetAbilitySystemComponent()->K2_GiveAbility(Item.GetData().AbilityClass, Item.Level);
		}
	}
	else
	{
		Item.AbilityHandle = FGameplayAbilitySpecHandle();
	}
	if(IsSelected())
	{
		if(IInventoryAgentInterface* InventoryAgent = Cast<IInventoryAgentInterface>(Inventory->GetOwnerActor()))
		{
			InventoryAgent->OnSelectedItemChange(GetItem());
		}
	}
}

void UInventorySlot::Replace(UInventorySlot* InSlot)
{
	auto tmpItem = GetItem();
	SetItem(InSlot->GetItem());
	InSlot->SetItem(tmpItem);
}

void UInventorySlot::SetItem(FAbilityItem& InItem, bool bRefresh)
{
	OnItemPreChange(InItem);
	FAbilityItem OldItem = Item;
	Item = InItem;
	Item.Count = FMath::Clamp(Item.Count, 0, GetMaxVolume(InItem));
	OnItemChanged(OldItem);
	if(bRefresh)
	{
		Refresh();
	}
}

void UInventorySlot::AddItem(FAbilityItem& InItem)
{
	if (Contains(InItem))
	{
		int32 tmpNum = InItem.Count - GetRemainVolume(InItem);
		Item.Count = FMath::Clamp(Item.Count + InItem.Count, 0, GetMaxVolume(InItem));
		InItem.Count = FMath::Max(tmpNum, 0);
		Refresh();
	}
	else
	{
		SetItem(InItem);
		InItem.Count -= GetMaxVolume(InItem);
	}
}

void UInventorySlot::SubItem(FAbilityItem& InItem)
{
	if (Contains(InItem))
	{
		int32 tmpNum = InItem.Count - Item.Count;
		Item.Count = FMath::Clamp(Item.Count - InItem.Count, 0, GetMaxVolume(InItem));
		InItem.Count = FMath::Max(tmpNum, 0);
		Refresh();
	}
}

void UInventorySlot::SplitItem(int32 InCount /*= -1*/)
{
	if (IsEmpty()) return;

	if(InCount == - 1) InCount = Item.Count;
	const int32 tmpCount = Item.Count / InCount;
	auto QueryItemInfo = Inventory->QueryItemByRange(EQueryItemType::Add, Item);
	for (int32 i = 0; i < InCount; i++)
	{
		FAbilityItem tmpItem = FAbilityItem(Item, tmpCount);
		Item.Count -= tmpItem.Count;
		for (int32 j = 0; j < QueryItemInfo.Slots.Num(); j++)
		{
			if (QueryItemInfo.Slots[j]->IsEmpty() && QueryItemInfo.Slots[j] != this)
			{
				QueryItemInfo.Slots[j]->SetItem(tmpItem);
				QueryItemInfo.Slots.RemoveAt(j);
				break;
			}
		}
	}
	Refresh();
}

void UInventorySlot::MoveItem(int32 InCount /*= -1*/)
{
	if (IsEmpty()) return;

	if (InCount == -1) InCount = Item.Count;
	FAbilityItem tmpItem = FAbilityItem(Item, InCount);

	if(Inventory->GetConnectInventory())
	{
		Inventory->GetConnectInventory()->AddItemByRange(tmpItem);
	}
	else
	{
		switch(GetSplitType())
		{
			case ESplitSlotType::Default:
			{
				Inventory->AddItemBySplitTypes(tmpItem, {ESplitSlotType::Shortcut, ESplitSlotType::Auxiliary});
				break;
			}
			case ESplitSlotType::Shortcut:
			{
				Inventory->AddItemBySplitTypes(tmpItem, {ESplitSlotType::Default, ESplitSlotType::Auxiliary});
				break;
			}
			case ESplitSlotType::Auxiliary:
			{
				Inventory->AddItemBySplitTypes(tmpItem, {ESplitSlotType::Default, ESplitSlotType::Shortcut});
				break;
			}
			case ESplitSlotType::Equip:
			case ESplitSlotType::Skill:
			{
				Inventory->AddItemBySplitTypes(tmpItem, {ESplitSlotType::Default, ESplitSlotType::Shortcut, ESplitSlotType::Auxiliary});
				break;
			}
			default: break;
		}
	}

	tmpItem.Count = InCount - tmpItem.Count;
	SubItem(tmpItem);
}

void UInventorySlot::UseItem(int32 InCount /*= -1*/)
{
	if (IsEmpty()) return;

	if(InCount == -1) InCount = Item.Count;

	if(ADWCharacter* Character = Cast<ADWCharacter>(GetInventory()->GetOwnerActor()))
	{
		Character->DoAction(EDWCharacterActionType::Use);
	}

	switch (Item.GetData().GetItemType())
	{
		case EAbilityItemType::Prop:
		{
			DON(i, InCount,
				if(ActiveItem())
				{
					auto tmpItem = FAbilityItem(Item, 1);
					SubItem(tmpItem);
				}
				else break;
			)
			break;
		}
		case EAbilityItemType::Equip:
		case EAbilityItemType::Skill:
		{
			AssembleItem();
			break;
		}
		default: break;
	}
}

void UInventorySlot::AssembleItem()
{
	if (IsEmpty()) return;

	switch (Item.GetData().GetItemType())
	{
		case EAbilityItemType::Equip:
		{
			Inventory->AddItemBySplitType(Item, ESplitSlotType::Equip); 
			Refresh();
			break;
		}
		case EAbilityItemType::Skill:
		{
			Inventory->AddItemBySplitType(Item, ESplitSlotType::Skill);
			Refresh();
			break;
		}
		default: break;
	}
}

void UInventorySlot::DischargeItem()
{
	if (IsEmpty()) return;

	switch (Item.GetData().GetItemType())
	{
		case EAbilityItemType::Equip:
		case EAbilityItemType::Skill:
		{
			Inventory->AddItemBySplitTypes(Item, {ESplitSlotType::Default, ESplitSlotType::Shortcut, ESplitSlotType::Auxiliary});
			Refresh();
			break;
		}
		default: break;
	}
}

void UInventorySlot::DiscardItem(int32 InCount /*= -1*/, bool bOffsetX /*= true*/)
{
	if (IsEmpty()) return;

	if(IAbilityVitalityInterface* Vitality = Cast<IAbilityVitalityInterface>(GetInventory()->GetOwnerActor()))
	{
		FAbilityItem tmpItem = FAbilityItem(Item, InCount);
		if(ADWCharacter* Character = Cast<ADWCharacter>(Vitality))
		{
			Character->DoAction(EDWCharacterActionType::Discard);
		}
		FVector tmpPos = Inventory->GetOwnerActor()->GetActorLocation() + FMath::RandPointInBox(FBox(FVector(-20.f, -20.f, -10.f), FVector(20.f, 20.f, 10.f)));
		if(bOffsetX) tmpPos += Inventory->GetOwnerActor()->GetActorForwardVector() * (Vitality->GetRadius() + 35.f);
		if(auto chunk = AVoxelModule::Get()->FindChunkByLocation(tmpPos))
		{
			UAbilityModuleBPLibrary::SpawnPickUp(tmpItem, tmpPos, chunk);
		}
		SubItem(tmpItem);
	}
}

bool UInventorySlot::ActiveItem()
{
	if(IsEmpty()) return false;

	if(IAbilityVitalityInterface* Vitality = Cast<IAbilityVitalityInterface>(GetInventory()->GetOwnerActor()))
	{
		if(Vitality->GetAbilitySystemComponent()->TryActivateAbility(Item.AbilityHandle))
		{
			OnInventorySlotActivated.Broadcast();
			for(auto Iter : GetInventory()->QueryItemByRange(EQueryItemType::Get, Item).Slots)
			{
				if(Iter != this)
				{
					Iter->OnInventorySlotActivated.Broadcast();
				}
			}
			return true;
		}
		else if(GetAbilityInfo().IsCooldownning())
		{
			UWidgetModuleBPLibrary::OpenUserWidget<UWidgetItemInfoBox>({ FParameter::MakeString(FString::Printf(TEXT("该%s处于冷却中！"), *UGlobalBPLibrary::GetEnumValueDisplayName(TEXT("EAbilityItemType"), (int32)Item.GetData().GetItemType()).ToString())) });
			return false;
		}
	}
    return false;
}

bool UInventorySlot::CancelItem()
{
	if(IsEmpty()) return false;
	
	if(IAbilityVitalityInterface* Vitality = Cast<IAbilityVitalityInterface>(GetInventory()->GetOwnerActor()))
	{
		Vitality->GetAbilitySystemComponent()->CancelAbilityHandle(Item.AbilityHandle);
		OnInventorySlotCanceled.Broadcast();
		return true;
	}
	return false;
}

void UInventorySlot::ClearItem()
{
	if (IsEmpty()) return;

	SetItem(FAbilityItem::Empty);
}

bool UInventorySlot::IsEmpty() const
{
	return Item.IsEmpty();
}

bool UInventorySlot::IsSelected() const
{
	return GetInventory()->GetSelectedSlot() == this;
}

bool UInventorySlot::IsMatched(bool bForce) const
{
	if(IsEmpty()) return false;

	return IsMatch(Item, bForce);
}

int32 UInventorySlot::GetSplitIndex(ESplitSlotType InSplitSlotType)
{
	return GetInventory()->GetSplitSlots(InSplitSlotType).Find(this);
}

int32 UInventorySlot::GetRemainVolume(FAbilityItem InItem) const
{
	const FAbilityItem TmpItem = InItem.IsValid() ? InItem : Item;
	return GetMaxVolume(TmpItem) - Item.Count;
}

int32 UInventorySlot::GetMaxVolume(FAbilityItem InItem) const
{
	const FAbilityItem TmpItem = InItem.IsValid() ? InItem : Item;
	return TmpItem.IsValid() ? TmpItem.GetData().MaxCount : 0;
}

FAbilityInfo UInventorySlot::GetAbilityInfo() const
{
	if(IsEmpty() || !Item.AbilityHandle.IsValid()) return FAbilityInfo();
	
	if(IAbilityActorInterface* AbilityActor = Inventory->GetOwnerActor<IAbilityActorInterface>())
	{
		if(UAbilitySystemComponentBase* AbilitySystemComp = Cast<UAbilitySystemComponentBase>(AbilityActor->GetAbilitySystemComponent()))
		{
			return AbilitySystemComp->GetAbilityInfoByHandle(Item.AbilityHandle);
		}
	}
	return FAbilityInfo();
}
