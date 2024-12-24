// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Slot/DWInventoryEquipSlot.h"

#include "Item/Equip/DWEquipData.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Character/DWCharacter.h"
#include "Item/Equip/Weapon/DWEquipWeaponData.h"

UDWInventoryEquipSlot::UDWInventoryEquipSlot()
{
}

void UDWInventoryEquipSlot::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);
}

void UDWInventoryEquipSlot::OnDespawn_Implementation(bool bRecovery) 
{
	Super::OnDespawn_Implementation(bRecovery);

	EquipPart = EDWEquipPart::Head;
}

void UDWInventoryEquipSlot::OnInitialize(UAbilityInventoryBase* InInventory, EAbilityItemType InLimitType, ESlotSplitType InSplitType, int32 InSlotIndex)
{
	Super::OnInitialize(InInventory, InLimitType, InSplitType, InSlotIndex);

	EquipPart = (EDWEquipPart)InSlotIndex;
}

void UDWInventoryEquipSlot::OnItemPreChange(FAbilityItem& InNewItem, bool bBroadcast)
{
	Super::OnItemPreChange(InNewItem, bBroadcast);
}

void UDWInventoryEquipSlot::OnItemChanged(FAbilityItem& InOldItem, bool bBroadcast)
{
	Super::OnItemChanged(InOldItem, bBroadcast);
}

bool UDWInventoryEquipSlot::MatchItemLimit(FAbilityItem InItem, bool bForce) const
{
	if(!Super::MatchItemLimit(InItem, bForce)) return false;
	
	if(EquipPart < EDWEquipPart::Primary) return InItem.GetData<UDWEquipData>().EquipPart == EquipPart;

	if(InItem.IsDataType<UDWEquipWeaponData>())
	{
		if(bForce)
		{
			EDWEquipPart OtherPart = EquipPart == EDWEquipPart::Primary ? EDWEquipPart::Secondary : EDWEquipPart::Primary;
			UAbilityInventorySlotBase* OtherSlot = Inventory->GetSlotBySplitTypeAndIndex(ESlotSplitType::Equip, (int32)OtherPart);
			if(OtherSlot != InItem.InventorySlot)
			{
				if(InItem.GetData<UDWEquipWeaponData>().WeaponHand == EDWWeaponHand::Both)
				{
					if(!OtherSlot->IsEmpty()) return false;
				}
				else if(!OtherSlot->IsEmpty())
				{
					if(OtherSlot->GetItem().GetData<UDWEquipWeaponData>().WeaponHand == EDWWeaponHand::Both) return false;
				}
			}
		}
		return true;
	}
	return false;
}

void UDWInventoryEquipSlot::Refresh()
{
	Super::Refresh();
}

bool UDWInventoryEquipSlot::ActiveItem(bool bPassive)
{
	if(!bPassive)
	{
		ADWCharacter* Character = Inventory->GetOwnerAgent<ADWCharacter>();
		if(Character && Character->GetControlMode() != EDWCharacterControlMode::Fighting) return false;
	}
	return Super::ActiveItem(bPassive);
}

void UDWInventoryEquipSlot::DeactiveItem(bool bPassive)
{
	Super::DeactiveItem(bPassive);
}
