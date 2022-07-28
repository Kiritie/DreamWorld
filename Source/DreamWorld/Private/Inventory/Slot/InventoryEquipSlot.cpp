// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Slot/InventoryEquipSlot.h"

#include "Item/Equip/DWEquipData.h"
#include "Inventory/Inventory.h"
#include "Character/DWCharacter.h"

UInventoryEquipSlot::UInventoryEquipSlot()
{
}

void UInventoryEquipSlot::InitSlot(UInventory* InOwner, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::InitSlot(InOwner, InItem, InLimitType, InSplitType);
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
	auto character = Cast<ADWCharacter>(Owner->GetOwnerActor());
	if (character != nullptr)
	{
		character->RefreshEquip(PartType, Item);
	}
}

void UInventoryEquipSlot::PreSet(FAbilityItem& InItem)
{
	Super::PreSet(InItem);
	CancelItem();
}

void UInventoryEquipSlot::EndSet()
{
	Super::EndSet();
	ActiveItem();
}

void UInventoryEquipSlot::Assemble()
{
	Owner->AdditionItemBySplitType(Item, ESplitSlotType::Equip); 
	Refresh();
}

void UInventoryEquipSlot::Discharge()
{
	Owner->AdditionItemBySplitType(Item, ESplitSlotType::Default);
	Owner->AdditionItemBySplitType(Item, ESplitSlotType::Shortcut);
	Owner->AdditionItemBySplitType(Item, ESplitSlotType::Auxiliary);
	Refresh();
}
