// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Slot/DWInventoryEquipSlot.h"

#include "Item/Equip/DWEquipData.h"
#include "Ability/Inventory/Inventory.h"
#include "Character/DWCharacter.h"

UDWInventoryEquipSlot::UDWInventoryEquipSlot()
{
}

void UDWInventoryEquipSlot::OnInitialize(UInventory* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::OnInitialize(InInventory, InItem, InLimitType, InSplitType);
}

void UDWInventoryEquipSlot::OnInitialize(UInventory* InInventory, FAbilityItem InItem, EAbilityItemType InLimitType, ESplitSlotType InSplitType, int32 InPartType)
{
	Super::OnInitialize(InInventory, InItem, InLimitType, InSplitType, InPartType);

	PartType = (EDWEquipPartType)InPartType;
}

void UDWInventoryEquipSlot::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void UDWInventoryEquipSlot::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();

	PartType = EDWEquipPartType::Head;
}

bool UDWInventoryEquipSlot::CheckSlot(FAbilityItem& InItem) const
{
	return Super::CheckSlot(InItem) && InItem.GetData<UDWEquipData>().PartType == PartType;
}

void UDWInventoryEquipSlot::Refresh()
{
	Super::Refresh();

	if (ADWCharacter* Character = GetInventory()->GetOwnerAgent<ADWCharacter>())
	{
		Character->RefreshEquip(PartType, Item);
	}
}

void UDWInventoryEquipSlot::OnItemPreChange(FAbilityItem& InNewItem)
{
	Super::OnItemPreChange(InNewItem);
}

void UDWInventoryEquipSlot::OnItemChanged(FAbilityItem& InOldItem)
{
	Super::OnItemChanged(InOldItem);
}
