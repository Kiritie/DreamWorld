// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/WidgetInventoryBox.h"

#include "Ability/Inventory/Inventory.h"
#include "Ability/Inventory/InventoryAgentInterface.h"
#include "Ability/Inventory/Slot/InventorySlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"

UWidgetInventoryBox::UWidgetInventoryBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InventoryBox");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::UIOnly;

	WidgetType = EWidgetType::Temporary;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	SetIsFocusable(true);

	DefaultSlotClass = LoadClass<UWidgetInventorySlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/WB_InventorySlot.WB_InventorySlot_C'"));

	UISlotDatas.Add(ESplitSlotType::Default);
	TargetUISlotDatas.Add(ESplitSlotType::Default);
}

void UWidgetInventoryBox::OnInitialize_Implementation(UObject* InOwner)
{
	if(OwnerObject == InOwner) return;
	
	Super::OnInitialize_Implementation(InOwner);
	
	if(DefaultContent && UISlotDatas.Contains(ESplitSlotType::Default))
	{
		const auto DefaultSlots = GetInventory()->GetSplitSlots<UInventorySlot>(ESplitSlotType::Default);
		if(UISlotDatas[ESplitSlotType::Default].Slots.Num() != DefaultSlots.Num())
		{
			DefaultContent->ClearChildren();
			UISlotDatas[ESplitSlotType::Default].Slots.Empty();
			for(int32 i = 0; i < DefaultSlots.Num(); i++)
			{
				if(UWidgetInventorySlot* DefaultSlot = CreateSubWidget<UWidgetInventorySlot>({ DefaultSlots[i] }, DefaultSlotClass))
				{
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
				UISlotDatas[ESplitSlotType::Default].Slots[i]->OnInitialize({ DefaultSlots[i] });
			}
		}
	}
}

void UWidgetInventoryBox::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	if(InParams.IsValidIndex(0))
	{
		TargetObject = InParams[0].GetObjectValue();
	}

	if(Cast<AVoxelInteractAuxiliary>(TargetObject))
	{
		Cast<AVoxelInteractAuxiliary>(TargetObject)->Open();
	}

	UInventory* TargetInventory = Cast<IInventoryAgentInterface>(TargetObject)->GetInventory();

	if(!TargetInventory) return;
	
	GetInventory()->SetConnectInventory(TargetInventory);

	if(TargetContent && TargetUISlotDatas.Contains(ESplitSlotType::Default))
	{
		const auto DefaultSlots = TargetInventory->GetSplitSlots<UInventorySlot>(ESplitSlotType::Default);
		if(TargetUISlotDatas[ESplitSlotType::Default].Slots.Num() != DefaultSlots.Num())
		{
			TargetContent->ClearChildren();
			TargetUISlotDatas[ESplitSlotType::Default].Slots.Empty();
			for(int32 i = 0; i < DefaultSlots.Num(); i++)
			{
				if(UWidgetInventorySlot* DefaultSlot = CreateSubWidget<UWidgetInventorySlot>({ DefaultSlots[i] }, DefaultSlotClass))
				{
					if(UWrapBoxSlot* WrapBoxSlot = TargetContent->AddChildToWrapBox(DefaultSlot))
					{
						WrapBoxSlot->SetPadding(FMargin(2.5f, 2.5f, 2.5f, 2.5f));
					}
					TargetUISlotDatas[ESplitSlotType::Default].Slots.Add(DefaultSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < TargetUISlotDatas[ESplitSlotType::Default].Slots.Num(); i++)
			{
				TargetUISlotDatas[ESplitSlotType::Default].Slots[i]->OnInitialize({ DefaultSlots[i] });
			}
		}
	}
}

void UWidgetInventoryBox::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	GetInventory()->SetConnectInventory(nullptr);
	
	if(Cast<AVoxelInteractAuxiliary>(TargetObject))
	{
		Cast<AVoxelInteractAuxiliary>(TargetObject)->Close();
	}
}

void UWidgetInventoryBox::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}
