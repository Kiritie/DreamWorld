// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/WidgetInventoryBox.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/AbilityInventoryAgentInterface.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
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

	UISlotDatas.Add(ESlotSplitType::Default);
	TargetUISlotDatas.Add(ESlotSplitType::Default);
}

void UWidgetInventoryBox::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
	
	if(DefaultContent && UISlotDatas.Contains(ESlotSplitType::Default))
	{
		const auto DefaultSlots = GetInventory()->GetSlotsBySplitType(ESlotSplitType::Default);
		if(UISlotDatas[ESlotSplitType::Default].Slots.Num() != DefaultSlots.Num())
		{
			DefaultContent->ClearChildren();
			UISlotDatas[ESlotSplitType::Default].Slots.Empty();
			for(int32 i = 0; i < DefaultSlots.Num(); i++)
			{
				if(UWidgetInventorySlot* DefaultSlot = CreateSubWidget<UWidgetInventorySlot>({ DefaultSlots[i] }, DefaultSlotClass))
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
}

void UWidgetInventoryBox::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	UAbilityInventoryBase* TargetInventory = nullptr;
	if(InParams.IsValidIndex(0))
	{
		TargetInventory = InParams[0].GetObjectValue<IAbilityInventoryAgentInterface>()->GetInventory();
	}

	if(!TargetInventory) return;
	
	GetInventory()->SetConnectInventory(TargetInventory);

	if(TargetContent && TargetUISlotDatas.Contains(ESlotSplitType::Default))
	{
		const auto DefaultSlots = TargetInventory->GetSlotsBySplitType(ESlotSplitType::Default);
		if(TargetUISlotDatas[ESlotSplitType::Default].Slots.Num() != DefaultSlots.Num())
		{
			TargetContent->ClearChildren();
			TargetUISlotDatas[ESlotSplitType::Default].Slots.Empty();
			for(int32 i = 0; i < DefaultSlots.Num(); i++)
			{
				if(UWidgetInventorySlot* DefaultSlot = CreateSubWidget<UWidgetInventorySlot>({ DefaultSlots[i] }, DefaultSlotClass))
				{
					if(UWrapBoxSlot* WrapBoxSlot = TargetContent->AddChildToWrapBox(DefaultSlot))
					{
						WrapBoxSlot->SetPadding(FMargin(2.5f, 2.5f, 2.5f, 2.5f));
					}
					TargetUISlotDatas[ESlotSplitType::Default].Slots.Add(DefaultSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < TargetUISlotDatas[ESlotSplitType::Default].Slots.Num(); i++)
			{
				TargetUISlotDatas[ESlotSplitType::Default].Slots[i]->OnInitialize({ DefaultSlots[i] });
			}
		}
	}
}

void UWidgetInventoryBox::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	GetInventory()->SetConnectInventory(nullptr);

	if(WidgetParams.IsValidIndex(0))
	{
		if(AVoxelInteractAuxiliary* InteractionAgent = WidgetParams[0].GetObjectValue<AVoxelInteractAuxiliary>())
		{
			GetOwnerObject<IInteractionAgentInterface>()->DoInteract((EInteractAction)EVoxelInteractAction::Close, InteractionAgent);
		}
	}
}

void UWidgetInventoryBox::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}
