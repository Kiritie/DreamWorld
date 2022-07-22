// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Inventory/Slot/InventoryShortcutSlot.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Global/GlobalBPLibrary.h"
#include "Inventory/Inventory.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetGameHUD.h"

UInventoryShortcutSlot::UInventoryShortcutSlot()
{
	LimitType = EAbilityItemType::Skill;
}

void UInventoryShortcutSlot::InitSlot(UInventory* InOwner, FAbilityItem InItem, EAbilityItemType InLimitType /* = EAbilityItemType::None */, ESplitSlotType InSplitType /*= ESplitSlotType::Default*/)
{
	Super::InitSlot(InOwner, InItem, InLimitType, InSplitType);
}

void UInventoryShortcutSlot::Refresh()
{
	Super::Refresh();
	if(IsSelected())
	{
		if(Item.GetData().EqualType(EAbilityItemType::Voxel))
		{
			UGlobalBPLibrary::GetPlayerCharacter()->SetGenerateVoxelItem(Item);
		}
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->RefreshActions();
	}
}

bool UInventoryShortcutSlot::IsSelected()
{
	return GetOwner()->GetSelectedSlot() == this;
}
