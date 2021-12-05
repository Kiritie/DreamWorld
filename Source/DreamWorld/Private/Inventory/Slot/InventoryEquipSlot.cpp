// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Slot/InventoryEquipSlot.h"
#include "Inventory/Inventory.h"
#include "Character/DWCharacter.h"

UInventoryEquipSlot::UInventoryEquipSlot()
{
	LimitType = EItemType::Equip;
	PartType = EEquipPartType::Head;
}

void UInventoryEquipSlot::InitSlot(UInventory* InOwner, FItem InItem, EItemType InLimitType /* = EItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::InitSlot(InOwner, InItem, InLimitType, InSplitType);
}

bool UInventoryEquipSlot::CheckSlot(FItem& InItem) const
{
	return Super::CheckSlot(InItem) && UDWHelper::LoadEquipData(InItem.ID).PartType == PartType;
}

void UInventoryEquipSlot::Refresh()
{
	Super::Refresh();
	auto character = Cast<ADWCharacter>(Owner->GetOwnerActor());
	if (character != nullptr)
	{
		character->RefreshEquip(PartType, this);
	}
}

void UInventoryEquipSlot::PreSet(FItem& InItem)
{
	Super::PreSet(InItem);
	CancelItem();
}

void UInventoryEquipSlot::EndSet()
{
	Super::EndSet();
	ActiveItem();
}
