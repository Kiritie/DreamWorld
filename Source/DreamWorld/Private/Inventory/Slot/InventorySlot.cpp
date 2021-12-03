// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/InventorySlot.h"
#include "Inventory/Inventory.h"
#include "World/WorldManager.h"
#include "World/Chunk.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Vitality/Vitality.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "Abilities/Item/DWItemAbility.h"

UInventorySlot::UInventorySlot()
{
	Item = FItem::Empty;
	Owner = nullptr;
	LimitType = EItemType::None;
	SplitType = ESplitSlotType::Default;
	AbilityHandle = FGameplayAbilitySpecHandle();
}

void UInventorySlot::InitSlot(UInventory* InOwner, FItem InItem, EItemType InLimitType /*= EItemType::None*/, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Item = InItem;
	Owner = InOwner;
	LimitType = InLimitType;
	SplitType = InSplitType;
}

bool UInventorySlot::CheckSlot(FItem& InItem) const
{
	return LimitType == EItemType::None || LimitType == InItem.GetData().Type;
}

bool UInventorySlot::CanPutIn(FItem& InItem) const
{
	return (IsEmpty() && CheckSlot(InItem)) || (Item.EqualType(InItem) && GetRemainVolume() > 0);
}

bool UInventorySlot::Contains(FItem& InItem) const
{
	return !IsEmpty() && Item.EqualType(InItem);
}

void UInventorySlot::Refresh()
{
	if (!IsEmpty() && Item.Count <= 0)
	{
		SetItem(FItem::Empty, false);
	}
	OnInventorySlotRefresh.Broadcast();
}

void UInventorySlot::PreSet(FItem& InItem)
{
	
}

void UInventorySlot::EndSet()
{
	if(Item.IsValid())
	{
		IVitality* vitality = Cast<IVitality>(Owner->GetOwnerActor());
		if (vitality && Item.GetData().AbilityClass)
		{
			AbilityHandle = vitality->AcquireAbility(Item.GetData().AbilityClass, Item.Level);
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

void UInventorySlot::SetItem(FItem& InItem, bool bRefresh)
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

void UInventorySlot::AddItem(FItem& InItem)
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

void UInventorySlot::SubItem(FItem& InItem)
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
		FItem tmpItem = FItem(Item, tmpCount);
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
	FItem tmpItem = FItem(Item, InCount);

	if(Owner->GetConnectInventory())
	{
		Owner->GetConnectInventory()->AdditionItemByRange(tmpItem);
	}
	else
	{
		switch (Item.GetData().Type)
		{
			case EItemType::Equip:
			{
				if(GetSplitType() != ESplitSlotType::Equip)
				{
					Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Equip);
				}
				break;
			}
			case EItemType::Skill:
			{
				if(GetSplitType() != ESplitSlotType::Skill)
				{
					Owner->AdditionItemBySplitType(tmpItem, ESplitSlotType::Skill);
				}
				break;
			}
			default: break;
		}
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
	
	switch (Item.GetData().Type)
	{
		case EItemType::Voxel:
		{
			if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwner()->GetOwnerActor()))
			{
				for(int32 i = 0; i < InCount; i ++)
				{
					FItem tmpItem = FItem(Item, 1);
					if(OwnerCharacter->UseItem(tmpItem))
					{
						OwnerCharacter->DoAction(ECharacterActionType::Use);
						SubItem(tmpItem);
					}
					else break;
				}
			}
			break;
		}
		case EItemType::Prop:
		{
			if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwner()->GetOwnerActor()))
			{
				for(int32 i = 0; i < InCount; i ++)
				{
					FItem tmpItem = FItem(Item, 1);
					if(OwnerCharacter->UseItem(tmpItem) && ActiveItem())
					{
						OwnerCharacter->DoAction(ECharacterActionType::Use);
						SubItem(tmpItem);
					}
					else break;
				}
			}
			break;
		}
		case EItemType::Equip:
		{
			if(GetSplitType() != ESplitSlotType::Equip)
			{
				MoveItem(InCount);
			}
			break;
		}
		case EItemType::Skill:
		{
			if(GetSplitType() != ESplitSlotType::Skill)
			{
				MoveItem(InCount);
			}
			else
			{
				ActiveItem();
			}
			break;
		}
		default: break;
	}
}

void UInventorySlot::DiscardItem(int InCount /*= -1*/)
{
	if (IsEmpty()) return;

	if (InCount == -1) InCount = Item.Count;
	FItem tmpItem = FItem(Item, InCount);
	auto chunk = AWorldManager::Get()->FindChunk(Owner->GetOwnerActor()->GetActorLocation());
	if (chunk != nullptr)
	{
		chunk->SpawnPickUp(tmpItem, Owner->GetOwnerActor()->GetActorLocation() + FMath::RandPointInBox(FBox(FVector(-20, -20, -10), FVector(20, 20, 10))));
	}
	if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwner()->GetOwnerActor()))
	{
		OwnerCharacter->DoAction(ECharacterActionType::Use);
	}
	SubItem(tmpItem);
}

bool UInventorySlot::ActiveItem()
{
	if(AbilityHandle.IsValid())
	{
		if(IVitality* Vitality = Cast<IVitality>(GetOwner()->GetOwnerActor()))
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
		if(IVitality* Vitality = Cast<IVitality>(GetOwner()->GetOwnerActor()))
		{
			Vitality->CancelAbility(AbilityHandle);
			return true;
		}
	}
	return false;
}

void UInventorySlot::ClearItem()
{
	if (IsEmpty()) return;

	SetItem(FItem::Empty);
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
	return Item == FItem::Empty;
}

int UInventorySlot::GetRemainVolume() const
{
	return GetMaxVolume() - Item.Count;
}

int UInventorySlot::GetMaxVolume() const
{
	return Item.IsValid() ? Item.GetData().MaxCount : 0;
}

FDWAbilityInfo UInventorySlot::GetAbilityInfo() const
{
	FDWAbilityInfo AbilityInfo;
	if(ADWCharacter* Character = Cast<ADWCharacter>(Owner->GetOwnerActor()))
	{
		Character->GetAbilityInfo(Item.GetData().AbilityClass, AbilityInfo);
	}
	return AbilityInfo;
}
