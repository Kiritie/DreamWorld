// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/DWCharacterInventory.h"
#include "Inventory/Slot/DWInventoryEquipSlot.h"
#include "Inventory/Slot/DWInventorySkillSlot.h"
#include "Ability/Inventory/Slot/AbilityInventoryEquipSlot.h"
#include "Item/Equip/DWEquipData.h"
#include "Character/DWCharacter.h"
#include "Widget/WidgetContextBox.h"
#include "Widget/WidgetModuleStatics.h"

UDWCharacterInventory::UDWCharacterInventory()
{
	EquipSlotClass = UDWInventoryEquipSlot::StaticClass();
	SkillSlotClass = UDWInventorySkillSlot::StaticClass();
}

void UDWCharacterInventory::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UDWCharacterInventory::ToData()
{
	return Super::ToData();
}

void UDWCharacterInventory::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FItemQueryInfo UDWCharacterInventory::QueryItemByRange(EItemQueryType InActionType, FAbilityItem InItem, int32 InStartIndex, int32 InEndIndex)
{
	return Super::QueryItemByRange(InActionType, InItem, InStartIndex, InEndIndex);
}

void UDWCharacterInventory::AddItemBySlots(FAbilityItem& InItem, const TArray<UAbilityInventorySlot*>& InSlots)
{
	FAbilityItem tmpItem = InItem;

	Super::AddItemBySlots(InItem, InSlots);

	tmpItem -= InItem;

	if(GetOwnerAgent<ADWCharacter>()->IsPlayer())
	{
		if(tmpItem.Count > 0)
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("获得: %s × %d"), *InItem.GetData().Name.ToString(), tmpItem.Count));
		}
		else
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("无法再获得: %s"), *InItem.GetData().Name.ToString()));
		}
	}
}

void UDWCharacterInventory::RemoveItemBySlots(FAbilityItem& InItem, const TArray<UAbilityInventorySlot*>& InSlots)
{
	if(GetOwnerAgent<ADWCharacter>()->IsPlayer())
	{
		//UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("失去: %s × %d"), *InItem.GetData().Name.ToString(), InItem.Count));
	}

	Super::RemoveItemBySlots(InItem, InSlots);
}

void UDWCharacterInventory::DiscardAllItem()
{
	Super::DiscardAllItem();
}

void UDWCharacterInventory::ClearAllItem()
{
	Super::ClearAllItem();
}

UAbilityInventoryEquipSlot* UDWCharacterInventory::GetEquipSlotByPartType(EDWEquipPartType InPartType)
{
	auto EquipSlots = GetSlotsBySplitType(ESlotSplitType::Equip);
	for(int32 i = 0; i < EquipSlots.Num(); i++)
	{
		if(EquipSlots[i]->GetItem().GetData<UDWEquipData>().PartType == InPartType)
		{
			return Cast<UAbilityInventoryEquipSlot>(EquipSlots[i]);
		}
	}
	return nullptr;
}
