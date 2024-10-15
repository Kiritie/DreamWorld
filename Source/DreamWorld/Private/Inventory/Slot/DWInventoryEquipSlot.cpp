// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Slot/DWInventoryEquipSlot.h"

#include "Item/Equip/DWEquipData.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Character/DWCharacter.h"
#include "Item/Equip/Weapon/DWEquipWeaponData.h"

UDWInventoryEquipSlot::UDWInventoryEquipSlot()
{
}

void UDWInventoryEquipSlot::OnInitialize(UAbilityInventoryBase* InInventory, EAbilityItemType InLimitType, ESlotSplitType InSplitType, int32 InSlotIndex)
{
	Super::OnInitialize(InInventory, InLimitType, InSplitType, InSlotIndex);

	PartType = (EDWEquipPartType)InSlotIndex;
}

void UDWInventoryEquipSlot::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);
}

void UDWInventoryEquipSlot::OnDespawn_Implementation(bool bRecovery) 
{
	Super::OnDespawn_Implementation(bRecovery);

	PartType = EDWEquipPartType::Head;
}

bool UDWInventoryEquipSlot::CheckSlot(FAbilityItem& InItem) const
{
	if(!Super::CheckSlot(InItem)) return false;
	
	bool bReturnValue = InItem.GetData<UDWEquipData>().PartType == PartType;
	if(InItem.IsDataType<UDWEquipWeaponData>())
	{
		UAbilityInventorySlotBase* OtherSlot = Inventory->GetSlotBySplitTypeAndIndex(ESlotSplitType::Equip, PartType == EDWEquipPartType::LeftHand ? (int32)EDWEquipPartType::RightHand : (int32)EDWEquipPartType::LeftHand);

		if(InItem.GetData<UDWEquipWeaponData>().HandType == EDWWeaponHandType::Both)
		{
			if(!OtherSlot->IsEmpty())
			{
				bReturnValue = false;
			}
		}
		else if(OtherSlot->GetItem().IsDataType<UDWEquipWeaponData>())
		{
			if(OtherSlot->GetItem().GetData<UDWEquipWeaponData>().HandType == EDWWeaponHandType::Both)
			{
				bReturnValue = false;
			}
		}
	}
	return bReturnValue;
}

void UDWInventoryEquipSlot::Refresh()
{
	Super::Refresh();
}

void UDWInventoryEquipSlot::OnItemPreChange(FAbilityItem& InNewItem)
{
	Super::OnItemPreChange(InNewItem);
}

void UDWInventoryEquipSlot::OnItemChanged(FAbilityItem& InOldItem)
{
	Super::OnItemChanged(InOldItem);
}
