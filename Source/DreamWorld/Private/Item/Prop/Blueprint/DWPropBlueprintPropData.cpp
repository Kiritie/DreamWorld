// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Prop/Blueprint/DWPropBlueprintPropData.h"

UDWPropBlueprintPropData::UDWPropBlueprintPropData()
{
	GeneratePropType = EDWPropType::None;
}

bool UDWPropBlueprintPropData::CanGenerateItem(UAbilityItemDataBase* InItemData) const
{
	if(!Super::CanGenerateItem(InItemData)) return false;

	if(UDWPropData* PropData = Cast<UDWPropData>(InItemData))
	{
		return PropData->PropType == GeneratePropType;
	}
	return false;
}
