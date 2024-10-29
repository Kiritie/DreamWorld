// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/WidgetInventoryPanel.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Common/CommonStatics.h"
#include "Asset/AssetModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Inventory/Slot/WidgetInventoryEquipSlot.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"

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

void UWidgetInventoryPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);

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
				if(UWidgetInventorySlot* DefaultSlot = CreateSubWidget<UWidgetInventorySlot>({ DefaultSlots[i] }, UISlotDatas[ESlotSplitType::Default].Class))
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
				UISlotDatas[ESlotSplitType::Default].Slots[i]->Init({ DefaultSlots[i] });
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
				if(UWidgetInventoryEquipSlot* EquipSlot = CreateSubWidget<UWidgetInventoryEquipSlot>({ EquipSlots[i] }, UISlotDatas[ESlotSplitType::Equip].Class))
				{
					EquipSlot->SetEquipPartType(UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/DreamWorld.EDWEquipPartType"), i));
					if(UGridSlot* GridSlot = i % 2 == 0 ? LeftEquipContent->AddChildToGrid(EquipSlot) : RightEquipContent->AddChildToGrid(EquipSlot))
					{
						GridSlot->SetPadding(FMargin(2.5f, 20.f, 2.5f, 2.5f));
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
				UISlotDatas[ESlotSplitType::Equip].Slots[i]->Init({ EquipSlots[i] });
			}
		}
	}
}

void UWidgetInventoryPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();
}

void UWidgetInventoryPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();
}

void UWidgetInventoryPanel::OnRefresh()
{
	Super::OnRefresh();
}
