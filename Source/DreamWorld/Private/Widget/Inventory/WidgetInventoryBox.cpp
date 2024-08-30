// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/WidgetInventoryBox.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/AbilityInventoryAgentInterface.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Asset/AssetModuleStatics.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/WidgetUIMask.h"
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

	WidgetZOrder = 1;

	SetIsFocusable(true);

	UISlotDatas.Add(ESlotSplitType::Default);
	TargetUISlotDatas.Add(ESlotSplitType::Default);
}

void UWidgetInventoryBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
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
}

void UWidgetInventoryBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();

	UAbilityInventoryBase* TargetInventory = InParams.IsValidIndex(0) ? InParams[0].GetObjectValue<IAbilityInventoryAgentInterface>()->GetInventory() : nullptr;

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
				if(UWidgetInventorySlot* DefaultSlot = CreateSubWidget<UWidgetInventorySlot>({ DefaultSlots[i] }, UAssetModuleStatics::GetStaticClass(FName("InventoryDefaultSlot"))))
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

void UWidgetInventoryBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
	
	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();

	GetInventory()->SetConnectInventory(nullptr);
}

void UWidgetInventoryBox::OnRefresh()
{
	Super::OnRefresh();
}
