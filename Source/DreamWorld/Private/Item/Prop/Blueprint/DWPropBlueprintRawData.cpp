// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Prop/Blueprint/DWPropBlueprintRawData.h"

#include "Item/Raw/DWRawData.h"

UDWPropBlueprintRawData::UDWPropBlueprintRawData()
{
	GenerateRawType = EDWRawType::None;
}

bool UDWPropBlueprintRawData::CanGenerateItem(UAbilityItemDataBase* InItemData) const
{
	if(!Super::CanGenerateItem(InItemData)) return false;

	if(UDWRawData* RawData = Cast<UDWRawData>(InItemData))
	{
		return RawData->RawType == GenerateRawType;
	}
	return false;
}
