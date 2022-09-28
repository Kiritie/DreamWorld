// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/WidgetInventoryPanel.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Global/GlobalBPLibrary.h"
#include "Inventory/CharacterInventory.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Widget/Inventory/Slot/WidgetInventoryEquipSlot.h"

UWidgetInventoryPanel::UWidgetInventoryPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;

	WidgetName = FName("InventoryPanel");
	ParentName = FName("GameHUD");
	InputMode = EInputMode::UIOnly;

	WidgetType = EWidgetType::Temporary;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	DefaultSlotClass = LoadClass<UWidgetInventorySlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/WB_InventorySlot.WB_InventorySlot_C'"));
	EquipSlotClass = LoadClass<UWidgetInventoryEquipSlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/WB_InventoryEquipSlot.WB_InventoryEquipSlot_C'"));

	UISlotDatas.Add(ESplitSlotType::Default);
	UISlotDatas.Add(ESplitSlotType::Equip);
}

void UWidgetInventoryPanel::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
	
	if(!GetInventory()) return;

	if(DefaultContent && UISlotDatas.Contains(ESplitSlotType::Default))
	{
		const auto DefaultSlots = GetInventory()->GetSplitSlots<UInventorySlot>(ESplitSlotType::Default);
		if(UISlotDatas[ESplitSlotType::Default].Slots.Num() != DefaultSlots.Num())
		{
			DefaultContent->ClearChildren();
			UISlotDatas[ESplitSlotType::Default].Slots.Empty();
			for(int32 i = 0; i < DefaultSlots.Num(); i++)
			{
				if(UWidgetInventorySlot* DefaultSlot = Cast<UWidgetInventorySlot>(UWidgetBlueprintLibrary::Create(this, DefaultSlotClass, nullptr)))
				{
					DefaultSlot->OnInitialize(DefaultSlots[i]);
					if(UWrapBoxSlot* WrapBoxSlot = DefaultContent->AddChildToWrapBox(DefaultSlot))
					{
						WrapBoxSlot->SetPadding(FMargin(2.5f, 2.5f, 2.5f, 2.5f));
					}
					UISlotDatas[ESplitSlotType::Default].Slots.Add(DefaultSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESplitSlotType::Default].Slots.Num(); i++)
			{
				UISlotDatas[ESplitSlotType::Default].Slots[i]->OnInitialize(DefaultSlots[i]);
			}
		}
	}
	if(LeftEquipContent && RightEquipContent && UISlotDatas.Contains(ESplitSlotType::Equip))
	{
		const auto EquipSlots = GetInventory()->GetSplitSlots<UInventorySlot>(ESplitSlotType::Equip);
		if(UISlotDatas[ESplitSlotType::Equip].Slots.Num() == 0)
		{
			for(int32 i = 0; i < EquipSlots.Num(); i++)
			{
				if(UWidgetInventoryEquipSlot* EquipSlot = Cast<UWidgetInventoryEquipSlot>(UWidgetBlueprintLibrary::Create(this, EquipSlotClass, nullptr)))
				{
					EquipSlot->OnInitialize(EquipSlots[i]);
					EquipSlot->SetEquipPartType(UGlobalBPLibrary::GetEnumValueDisplayName(TEXT("EEquipPartType"), i));
					if(UGridSlot* GridSlot = i % 2 == 0 ? LeftEquipContent->AddChildToGrid(EquipSlot) : RightEquipContent->AddChildToGrid(EquipSlot))
					{
						GridSlot->SetPadding(FMargin(2.5f, 2.5f, 2.5f, 2.5f));
						GridSlot->SetRow(i / 2);
					}
					UISlotDatas[ESplitSlotType::Equip].Slots.Add(EquipSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESplitSlotType::Equip].Slots.Num(); i++)
			{
				UISlotDatas[ESplitSlotType::Equip].Slots[i]->OnInitialize(EquipSlots[i]);
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
