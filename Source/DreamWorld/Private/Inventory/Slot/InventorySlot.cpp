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
	if(IsSelected())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->RefreshActions();
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
	if(IInventoryAgentInterface* InventoryAgent = Cast<IInventoryAgentInterface>(Inventory->GetOwnerActor()))
	{
		InventoryAgent->OnSelectedItemChange(GetItem());
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
		int tmpNum = InItem.Count - GetRemainVolume(InItem);
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
		int tmpNum = InItem.Count - Item.Count;
		Item.Count = FMath::Clamp(Item.Count - InItem.Count, 0, GetMaxVolume(InItem));
		InItem.Count = FMath::Max(tmpNum, 0);
		Refresh();
	}
}

void UInventorySlot::SplitItem(int InCount /*= -1*/)
{
	if (IsEmpty()) return;

	if(InCount == - 1) InCount = Item.Count;
	const int tmpCount = Item.Count / InCount;
	auto QueryItemInfo = Inventory->QueryItemByRange(EQueryItemType::Add, Item);
	for (int i = 0; i < InCount; i++)
	{
		FAbilityItem tmpItem = FAbilityItem(Item, tmpCount);
		Item.Count -= tmpItem.Count;
		for (int j = 0; j < QueryItemInfo.Slots.Num(); j++)
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

void UInventorySlot::MoveItem(int InCount /*= -1*/)
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
				Inventory->AddItemBySplitType(tmpItem, ESplitSlotType::Shortcut);
				Inventory->AddItemBySplitType(tmpItem, ESplitSlotType::Auxiliary);
				break;
			}
			case ESplitSlotType::Shortcut:
			{
				Inventory->AddItemBySplitType(tmpItem, ESplitSlotType::Default);
				Inventory->AddItemBySplitType(tmpItem, ESplitSlotType::Auxiliary);
				break;
			}
			case ESplitSlotType::Auxiliary:
			{
				Inventory->AddItemBySplitType(tmpItem, ESplitSlotType::Default);
				Inventory->AddItemBySplitType(tmpItem, ESplitSlotType::Shortcut);
				break;
			}
			case ESplitSlotType::Equip:
			case ESplitSlotType::Skill:
			{
				Inventory->AddItemBySplitType(tmpItem, ESplitSlotType::Default);
				Inventory->AddItemBySplitType(tmpItem, ESplitSlotType::Shortcut);
				Inventory->AddItemBySplitType(tmpItem, ESplitSlotType::Auxiliary);
				break;
			}
			default: break;
		}
	}

	tmpItem.Count = InCount - tmpItem.Count;
	SubItem(tmpItem);
}

void UInventorySlot::UseItem(int InCount /*= -1*/)
{
	if (IsEmpty()) return;

	if(InCount == -1) InCount = Item.Count;

	ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetInventory()->GetOwnerActor());

	if(!OwnerCharacter || OwnerCharacter->DoAction(EDWCharacterActionType::Use))
	{
		switch (Item.GetData().GetItemType())
		{
			case EAbilityItemType::Prop:
			{
				for(int32 i = 0; i < InCount; i ++)
				{
					auto tmpItem = FAbilityItem(Item, 1);
					if(ActiveItem())
					{
						SubItem(tmpItem);
					}
					else break;
				}
				break;
			}
			default: break;
		}
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
		{
			Inventory->AddItemBySplitType(Item, ESplitSlotType::Default);
			Inventory->AddItemBySplitType(Item, ESplitSlotType::Shortcut);
			Inventory->AddItemBySplitType(Item, ESplitSlotType::Auxiliary);
			Refresh();
			break;
		}
		case EAbilityItemType::Skill:
		{
			Inventory->AddItemBySplitType(Item, ESplitSlotType::Default);
			Inventory->AddItemBySplitType(Item, ESplitSlotType::Shortcut);
			Inventory->AddItemBySplitType(Item, ESplitSlotType::Auxiliary);
			Refresh();
			break;
		}
		default: break;
	}
}

void UInventorySlot::DiscardItem(int InCount /*= -1*/)
{
	if (IsEmpty()) return;

	ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetInventory()->GetOwnerActor());

	if(!OwnerCharacter || OwnerCharacter->DoAction(EDWCharacterActionType::Discard))
	{
		FAbilityItem tmpItem = FAbilityItem(Item, InCount);
		if (auto chunk = AMainModule::GetModuleByClass<AVoxelModule>()->FindChunkByLocation(Inventory->GetOwnerActor()->GetActorLocation()))
		{
			UAbilityModuleBPLibrary::SpawnPickUp(tmpItem, Inventory->GetOwnerActor()->GetActorLocation() + FMath::RandPointInBox(FBox(FVector(-20, -20, -10), FVector(20, 20, 10))), chunk);
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
			return true;
		}
		else if(GetAbilityInfo().CooldownRemaining > 0.f)
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

int UInventorySlot::GetRemainVolume(FAbilityItem InItem) const
{
	const FAbilityItem TmpItem = InItem.IsValid() ? InItem : Item;
	return GetMaxVolume(TmpItem) - Item.Count;
}

int UInventorySlot::GetMaxVolume(FAbilityItem InItem) const
{
	const FAbilityItem TmpItem = InItem.IsValid() ? InItem : Item;
	return TmpItem.IsValid() ? TmpItem.GetData().MaxCount : 0;
}

FAbilityInfo UInventorySlot::GetAbilityInfo() const
{
	if(IsEmpty()) return FAbilityInfo();
	
	if(ADWCharacter* Character = Cast<ADWCharacter>(Inventory->GetOwnerActor()))
	{
		return Character->GetAbilitySystemComponent<UAbilitySystemComponentBase>()->GetAbilityInfoByHandle(Item.AbilityHandle);
	}
	return FAbilityInfo();
}
