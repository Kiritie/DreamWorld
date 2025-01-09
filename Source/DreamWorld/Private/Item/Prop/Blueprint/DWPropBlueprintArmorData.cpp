// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Prop/Blueprint/DWPropBlueprintArmorData.h"

#include "Item/Equip/Armor/DWEquipArmorData.h"

UDWPropBlueprintArmorData::UDWPropBlueprintArmorData()
{
}

bool UDWPropBlueprintArmorData::CanGenerateItem(UAbilityItemDataBase* InItemData) const
{
	if(!Super::CanGenerateItem(InItemData)) return false;

	if(Cast<UDWEquipArmorData>(InItemData))
	{
		return true;
	}
	return false;
}
