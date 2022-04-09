// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/InventorySlot.h"

#include "Ability/Item/ItemAbilityBase.h"
#include "Ability/Item/ItemDataBase.h"
#include "Inventory/Inventory.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"

UInventorySlot::UInventorySlot()
{
	Item = FAbilityItem::Empty;
	Owner = nullptr;
	LimitType = EAbilityItemType::None;
	SplitType = ESplitSlotType::Default;
	AbilityHandle = FGameplayAbilitySpecHandle();
}

void UInventorySlot::InitSlot(UInventory* InOwner, FAbilityItem InItem, EAbilityItemType InLimitType /*= EAbilityItemType::None*/, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Item = InItem;
	Owner = InOwner;
	LimitType = InLimitType;
	SplitType = InSplitType;
}

bool UInventorySlot::CheckSlot(FAbilityItem& InItem) const
{
	return LimitType == EAbilityItemType::None || InItem.GetData()->EqualType(LimitType);
}

bool UInventorySlot::CanPutIn(FAbilityItem& InItem) const
{
	return (IsEmpty() && CheckSlot(InItem)) || (Item.EqualType(InItem) && GetRemainVolume() > 0);
}

bool UInventorySlot::Contains(FAbilityItem& InItem) const
{
	return !IsEmpty() && Item.EqualType(InItem);
}

void UInventorySlot::Refresh()
{
	if (!IsEmpty() && Item.Count <= 0)
	{
		SetItem(FAbilityItem::Empty, false);
	}
	OnInventorySlotRefresh.Broadcast();
}

void UInventorySlot::PreSet(FAbilityItem& InItem)
{
	
}

void UInventorySlot::EndSet()
{
	if(Item.IsValid())
	{
		IAbilityVitalityInterface* vitality = Cast<IAbilityVitalityInterface>(Owner->GetOwnerActor());
		if (vitality && Item.GetData()->AbilityClass)
		{
			AbilityHandle = vitality->AcquireAbility(Item.GetData()->AbilityClass, Item.Level);
		}
	}
	else
	{
		AbilityHandle = FGameplayAbilitySpecHandle();
	}
}

void UInventorySlot::Replace(UInventorySlot* InSlot)
{
	auto tmpItem = GetItem();
	SetItem(InSlot->GetItem());
	InSlot->SetItem(tmpItem);

	// const auto abilityHandle = GetAbilityHandle();
	// SetAbilityHandle(InSlot->GetAbilityHandle());
	// InSlot->SetAbilityHandle(abilityHandle);

	const auto cooldownInfo = GetCooldownInfo();
	SetCooldownInfo(InSlot->GetCooldownInfo());
	InSlot->SetCooldownInfo(cooldownInfo);
}

void UInventorySlot::SetItem(FAbilityItem& InItem, bool bRefresh)
{
	PreSet(InItem);
	Item = InItem;
	Item.Count = FMath::Clamp(Item.Count, 0, GetMaxVolume());
	EndSet();
	if(bRefresh)
	{
		Refresh();
	}
}

void UInventorySlot::AddItem(FAbilityItem& InItem)
{
	if (Contains(InItem))
	{
		int tmpNum = InItem.Count - GetRemainVolume();
		Item.Count = FMath::Clamp(Item.Count + InItem.Count, 0, GetMaxVolume());
		InItem.Count = tmpNum;
		Refresh();
	}
	else
	{
		SetItem(InItem);
		InItem.Count -= GetMaxVolume();
	}
}

void UInventorySlot::SubItem(FAbilityItem& InItem)
{
	if (Contains(InItem))
	{
		int tmpNum = InItem.Count - Item.Count;
		Item.Count = FMath::Clamp(Item.Count - InItem.Count, 0, GetMaxVolume());
		InItem.Count = tmpNum;
		Refresh();
	}
}

void UInventorySlot::SplitItem(int InCount /*= -1*/)
{
	if (IsEmpty()) return;

	if(InCount == - 1) InCount = Item.Count;
	const int tmpCount = Item.Count / InCount;
	auto QueryItemInfo = Owner->GetItemInfoByRange(EQueryItemType::Addition, Item);
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

	if(Owner->GetConnectInventory())
	{
		Owner->GetConnectInventory()->AdditionItemByRange(tmpItem);
	}
	else
	{
		switch(GetSplitType())
		{
			case ESplitSlotType::Default:
			{
				Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Shortcut);
				Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Auxiliary);
				break;
			}
			case ESplitSlotType::Shortcut:
			{
				Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Default);
				Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Auxiliary);
				break;
			}
			case ESplitSlotType::Auxiliary:
			{
				Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Default);
				Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Shortcut);
				break;
			}
			case ESplitSlotType::Equip:
			case ESplitSlotType::Skill:
			{
				Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Default);
				Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Shortcut);
				Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Auxiliary);
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

	if (InCount == -1) InCount = Item.Count;
	
	if(Item.GetData()->EqualType(EAbilityItemType::Voxel))
	{
		if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwner()->GetOwnerActor()))
		{
			for(int32 i = 0; i < InCount; i ++)
			{
				FAbilityItem tmpItem = FAbilityItem(Item, 1);
				if(OwnerCharacter->UseItem(tmpItem))
				{
					OwnerCharacter->DoAction(EDWCharacterActionType::Use);
					SubItem(tmpItem);
				}
				else break;
			}
		}
	}
	else if(Item.GetData()->EqualType(EAbilityItemType::Prop))
	{
		if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwner()->GetOwnerActor()))
		{
			for(int32 i = 0; i < InCount; i ++)
			{
				FAbilityItem tmpItem = FAbilityItem(Item, 1);
				if(OwnerCharacter->UseItem(tmpItem) && ActiveItem())
				{
					OwnerCharacter->DoAction(EDWCharacterActionType::Use);
					SubItem(tmpItem);
				}
				else break;
			}
		}
	}
	else if(Item.GetData()->EqualType(EAbilityItemType::Equip))
	{
		if(GetSplitType() != ESplitSlotType::Equip)
		{
			Owner->AdditionItemBySplitType(Item, ESplitSlotType::Equip); 
			Refresh();
		}
	}
	else if(Item.GetData()->EqualType(EAbilityItemType::Skill))
	{
		if(GetSplitType() != ESplitSlotType::Skill)
		{
			Owner->AdditionItemBySplitType(Item, ESplitSlotType::Skill);
			Refresh();
		}
		else
		{
			ActiveItem();
		}
	}
}

void UInventorySlot::DiscardItem(int InCount /*= -1*/)
{
	if (IsEmpty()) return;

	if (InCount == -1) InCount = Item.Count;
	FAbilityItem tmpItem = FAbilityItem(Item, InCount);
	auto chunk = AVoxelModule::Get()->FindChunk(Owner->GetOwnerActor()->GetActorLocation());
	if (chunk != nullptr)
	{
		chunk->SpawnPickUp(tmpItem, Owner->GetOwnerActor()->GetActorLocation() + FMath::RandPointInBox(FBox(FVector(-20, -20, -10), FVector(20, 20, 10))));
	}
	if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwner()->GetOwnerActor()))
	{
		OwnerCharacter->DoAction(EDWCharacterActionType::Use);
	}
	SubItem(tmpItem);
}

bool UInventorySlot::ActiveItem()
{
	if(AbilityHandle.IsValid())
	{
		if(IAbilityVitalityInterface* Vitality = Cast<IAbilityVitalityInterface>(GetOwner()->GetOwnerActor()))
		{
			return Vitality->ActiveAbility(AbilityHandle);
		}
	}
    return false;
}

bool UInventorySlot::CancelItem()
{
	if(AbilityHandle.IsValid())
	{
		if(IAbilityVitalityInterface* Vitality = Cast<IAbilityVitalityInterface>(GetOwner()->GetOwnerActor()))
		{
			Vitality->CancelAbilityByHandle(AbilityHandle);
			return true;
		}
	}
	return false;
}

void UInventorySlot::ClearItem()
{
	if (IsEmpty()) return;

	SetItem(FAbilityItem::Empty);
}

void UInventorySlot::StartCooldown()
{
	CooldownInfo.bCooldowning = true;
	CooldownInfo.TotalTime = GetAbilityInfo().Cooldown;
	CooldownInfo.RemainTime = CooldownInfo.TotalTime;
}

void UInventorySlot::StopCooldown()
{
	CooldownInfo.bCooldowning = false;
	CooldownInfo.RemainTime = 0.f;
}

void UInventorySlot::RefreshCooldown(float DeltaSeconds)
{
	if(CooldownInfo.bCooldowning)
	{
		CooldownInfo.RemainTime -= DeltaSeconds;
		if(CooldownInfo.RemainTime <= 0.f)
		{
			StopCooldown();
		}
	}
	OnInventorySlotCooldownRefresh.Broadcast();
}

bool UInventorySlot::IsEmpty() const
{
	return Item == FAbilityItem::Empty;
}

int UInventorySlot::GetRemainVolume() const
{
	return GetMaxVolume() - Item.Count;
}

int UInventorySlot::GetMaxVolume() const
{
	return Item.IsValid() ? Item.GetData()->MaxCount : 0;
}

FAbilityInfo UInventorySlot::GetAbilityInfo() const
{
	FAbilityInfo AbilityInfo;
	if(ADWCharacter* Character = Cast<ADWCharacter>(Owner->GetOwnerActor()))
	{
		Character->GetAbilityInfo(Item.GetData()->AbilityClass, AbilityInfo);
	}
	return AbilityInfo;
}
