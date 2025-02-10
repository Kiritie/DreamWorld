// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/DWCommonStatics.h"

EDWEquipPart UDWCommonStatics::GetWeaponPartInSameGroup(EDWEquipPart InEquipPart)
{
	switch (InEquipPart)
	{
		case EDWEquipPart::Primary:
			return EDWEquipPart::Secondary;
		case EDWEquipPart::Secondary:
			return EDWEquipPart::Primary;
		case EDWEquipPart::Primary2:
			return EDWEquipPart::Secondary2;
		case EDWEquipPart::Secondary2:
			return EDWEquipPart::Primary2;
		case EDWEquipPart::Primary3:
			return EDWEquipPart::Secondary3;
		case EDWEquipPart::Secondary3:
			return EDWEquipPart::Primary3;
		default: break;
	}
	return EDWEquipPart::Primary;
}

TArray<EDWEquipPart> UDWCommonStatics::GetWeaponPartsByGroup(EDWWeaponGroup InWeaponGroup)
{
	switch (InWeaponGroup)
	{
		case EDWWeaponGroup::Group1:
			return { EDWEquipPart::Primary, EDWEquipPart::Secondary };
		case EDWWeaponGroup::Group2:
			return { EDWEquipPart::Primary2, EDWEquipPart::Secondary2 };
		case EDWWeaponGroup::Group3:
			return { EDWEquipPart::Primary3, EDWEquipPart::Secondary3 };
	}
	return {};
}
