// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Slot/InventoryEquipSlot.h"

#include "Item/Equip/DWEquipData.h"
#include "Inventory/Inventory.h"
#include "Character/DWCharacter.h"

UInventoryEquipSlot::UInventoryEquipSlot()
{
}

void UInventoryEquipSlot::OnInitialize(UInventory* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::OnInitialize(InInventory, InItem, InLimitType, InSplitType);
}

void UInventoryEquipSlot::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void UInventoryEquipSlot::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();

	PartType = EDWEquipPartType::Head;
}

bool UInventoryEquipSlot::CheckSlot(FAbilityItem& InItem) const
{
	return Super::CheckSlot(InItem) && InItem.GetData<UDWEquipData>().PartType == PartType;
}

void UInventoryEquipSlot::Refresh()
{
	Super::Refresh();
	auto character = Cast<ADWCharacter>(Inventory->GetOwnerActor());
	if (character != nullptr)
	{
		character->RefreshEquip(PartType, Item);
	}
}

void UInventoryEquipSlot::OnItemPreChange(FAbilityItem& InNewItem)
{
	Super::OnItemPreChange(InNewItem);
	CancelItem();
}

void UInventoryEquipSlot::OnItemChanged(FAbilityItem& InOldItem)
{
	Super::OnItemChanged(InOldItem);
	ActiveItem();
}

void UInventoryEquipSlot::Assemble()
{
	Inventory->AddItemBySplitType(Item, ESplitSlotType::Equip); 
	Refresh();
}

void UInventoryEquipSlot::Discharge()
{
	Inventory->AddItemBySplitType(Item, ESplitSlotType::Default);
	Inventory->AddItemBySplitType(Item, ESplitSlotType::Shortcut);
	Inventory->AddItemBySplitType(Item, ESplitSlotType::Auxiliary);
	Refresh();
}
