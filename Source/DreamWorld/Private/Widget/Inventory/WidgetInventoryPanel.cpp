// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/WidgetInventoryPanel.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Common/CommonBPLibrary.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"
#include "Widget/Inventory/Slot/WidgetInventoryEquipSlot.h"

UWidgetInventoryPanel::UWidgetInventoryPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InventoryPanel");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::UIOnly;

	WidgetType = EWidgetType::Temporary;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	WidgetZOrder = 1;

	SetIsFocusable(true);

	UISlotDatas.Add(ESlotSplitType::Default);
	UISlotDatas.Add(ESlotSplitType::Equip);
}

void UWidgetInventoryPanel::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);

	if(!InOwner) return;

	if(DefaultContent && UISlotDatas.Contains(ESlotSplitType::Default))
	{
		const auto DefaultSlots = GetInventory()->GetSlotsBySplitType(ESlotSplitType::Default);
		if(UISlotDatas[ESlotSplitType::Default].Slots.Num() != DefaultSlots.Num())
		{
			DefaultContent->ClearChildren();
			UISlotDatas[ESlotSplitType::Default].Slots.Empty();
			for(int32 i = 0; i < DefaultSlots.Num(); i++)
			{
				if(UWidgetInventorySlot* DefaultSlot = CreateSubWidget<UWidgetInventorySlot>({ DefaultSlots[i] }, UAssetModuleBPLibrary::GetStaticClass(FName("InventoryDefaultSlot"))))
				{
					if(UWrapBoxSlot* WrapBoxSlot = DefaultContent->AddChildToWrapBox(DefaultSlot))
					{
						WrapBoxSlot->SetPadding(FMargin(2.5f, 2.5f, 2.5f, 2.5f));
					}
					UISlotDatas[ESlotSplitType::Default].Slots.Add(DefaultSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESlotSplitType::Default].Slots.Num(); i++)
			{
				UISlotDatas[ESlotSplitType::Default].Slots[i]->OnInitialize({ DefaultSlots[i] });
			}
		}
	}
	if(LeftEquipContent && RightEquipContent && UISlotDatas.Contains(ESlotSplitType::Equip))
	{
		const auto EquipSlots = GetInventory()->GetSlotsBySplitType(ESlotSplitType::Equip);
		if(UISlotDatas[ESlotSplitType::Equip].Slots.Num() != EquipSlots.Num())
		{
			LeftEquipContent->ClearChildren();
			RightEquipContent->ClearChildren();
			UISlotDatas[ESlotSplitType::Equip].Slots.Empty();
			for(int32 i = 0; i < EquipSlots.Num(); i++)
			{
				if(UWidgetInventoryEquipSlot* EquipSlot = CreateSubWidget<UWidgetInventoryEquipSlot>({ EquipSlots[i] }, UAssetModuleBPLibrary::GetStaticClass(FName("InventoryEquipSlot"))))
				{
					EquipSlot->SetEquipPartType(UCommonBPLibrary::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EEquipPartType"), i));
					if(UGridSlot* GridSlot = i % 2 == 0 ? LeftEquipContent->AddChildToGrid(EquipSlot) : RightEquipContent->AddChildToGrid(EquipSlot))
					{
						GridSlot->SetPadding(FMargin(2.5f, 2.5f, 2.5f, 2.5f));
						GridSlot->SetRow(i / 2);
					}
					UISlotDatas[ESlotSplitType::Equip].Slots.Add(EquipSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESlotSplitType::Equip].Slots.Num(); i++)
			{
				UISlotDatas[ESlotSplitType::Equip].Slots[i]->OnInitialize({ EquipSlots[i] });
			}
		}
	}
}

void UWidgetInventoryPanel::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetInventoryPanel::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

void UWidgetInventoryPanel::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}
