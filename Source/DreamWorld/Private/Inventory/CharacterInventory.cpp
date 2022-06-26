// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/CharacterInventory.h"

#include "Character/DWCharacter.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"

UCharacterInventory::UCharacterInventory()
{
	
}

void UCharacterInventory::Initialize(AActor* InOwner, TMap<ESplitSlotType, FSplitSlotInfo> InSplitInfos)
{
	Super::Initialize(InOwner, InSplitInfos);
}

void UCharacterInventory::LoadData(FInventorySaveData InInventoryData, AActor* InOwner)
{
	Super::LoadData(InInventoryData, InOwner);
}

FInventorySaveData UCharacterInventory::ToData()
{
	return Super::ToData();
}

FQueryItemInfo UCharacterInventory::GetItemInfoByRange(EQueryItemType InActionType, FAbilityItem InItem, int32 InStartIndex, int32 InEndIndex)
{
	if (InStartIndex == -1 && HasSplitSlotInfo(ESplitSlotType::Shortcut) && GetOwnerCharacter() && GetOwnerCharacter()->IsPlayer())
	{
		InStartIndex = GetSplitSlotInfo(ESplitSlotType::Shortcut).StartIndex + UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlotIndex();
	}
	return Super::GetItemInfoByRange(InActionType, InItem, InStartIndex, InEndIndex);
}

void UCharacterInventory::DiscardAllItem()
{
	Super::DiscardAllItem();
}

void UCharacterInventory::ClearAllItem()
{
	Super::ClearAllItem();
}

ADWCharacter* UCharacterInventory::GetOwnerCharacter() const
{
	return Cast<ADWCharacter>(OwnerActor);
}
