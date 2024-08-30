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
#include "Common/CommonStatics.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Asset/AssetModuleStatics.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
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
				if(UWidgetInventorySlot* DefaultSlot = CreateSubWidget<UWidgetInventorySlot>({ DefaultSlots[i] }, UAssetModuleStatics::GetStaticClass(FName("InventoryDefaultSlot"))))
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
				if(UWidgetInventoryEquipSlot* EquipSlot = CreateSubWidget<UWidgetInventoryEquipSlot>({ EquipSlots[i] }, UAssetModuleStatics::GetStaticClass(FName("InventoryEquipSlot"))))
				{
					EquipSlot->SetEquipPartType(UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EEquipPartType"), i));
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

void UWidgetInventoryPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetInventoryPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

void UWidgetInventoryPanel::OnRefresh()
{
	Super::OnRefresh();

	if(IAbilityActorInterface* AbilityActor = GetOwnerObject<IAbilityActorInterface>())
	{
		AbilityActor->RefreshAttributes();
	}
}
