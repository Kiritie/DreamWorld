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
#include "Inventory/CharacterInventory.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Tool/GlobalToolsBPLibrary.h"
#include "Widget/Inventory/Slot/WidgetInventoryEquipSlot.h"

UWidgetInventoryPanel::UWidgetInventoryPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InventoryPanel");
	WidgetType = EWidgetType::Temporary;
	InputMode = EInputMode::GameAndUI;

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
					DefaultSlot->InitSlot(DefaultSlots[i]);
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
				UISlotDatas[ESplitSlotType::Default].Slots[i]->InitSlot(DefaultSlots[i]);
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
					EquipSlot->InitSlot(EquipSlots[i]);
					EquipSlot->SetEquipPartType(UGlobalToolsBPLibrary::GetEnumValueDisplayName(TEXT("EEquipPartType"), i));
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
				UISlotDatas[ESplitSlotType::Equip].Slots[i]->InitSlot(EquipSlots[i]);
			}
		}
	}
}
