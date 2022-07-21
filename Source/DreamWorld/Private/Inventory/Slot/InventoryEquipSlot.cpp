// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Slot/InventoryEquipSlot.h"

#include "Item/Equip/DWEquipData.h"
#include "Inventory/Inventory.h"
#include "Character/DWCharacter.h"

UInventoryEquipSlot::UInventoryEquipSlot()
{
	LimitType = EAbilityItemType::Equip;
	PartType = EDWEquipPartType::Head;
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
		character->RefreshEquip(PartType, this);
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
