// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/CharacterInventory.h"

#include "Character/DWCharacter.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"

UCharacterInventory::UCharacterInventory()
{
	
}

void UCharacterInventory::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	Super::LoadData(InSaveData, bForceMode);
}

FSaveData* UCharacterInventory::ToData()
{
	return Super::ToData();
}

void UCharacterInventory::UnloadData(bool bForceMode)
{
	Super::UnloadData(bForceMode);
}

FQueryItemInfo UCharacterInventory::QueryItemByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex, int32 InEndIndex)
{
	return Super::QueryItemByRange(InActionType, InItem, InStartIndex, InEndIndex);
}

void UCharacterInventory::DiscardAllItem()
{
	Super::DiscardAllItem();
}

void UCharacterInventory::ClearAllItem()
{
	Super::ClearAllItem();
}
