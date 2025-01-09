// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Prop/Blueprint/DWPropBlueprintWeaponData.h"

#include "Item/Equip/Weapon/DWEquipWeaponData.h"

UDWPropBlueprintWeaponData::UDWPropBlueprintWeaponData()
{
	GenerateWeaponType = EDWWeaponType::Sword;
}

bool UDWPropBlueprintWeaponData::CanGenerateItem(UAbilityItemDataBase* InItemData) const
{
	if(!Super::CanGenerateItem(InItemData)) return false;

	if(UDWEquipWeaponData* WeaponData = Cast<UDWEquipWeaponData>(InItemData))
	{
		return WeaponData->WeaponType == GenerateWeaponType;
	}
	return false;
}
