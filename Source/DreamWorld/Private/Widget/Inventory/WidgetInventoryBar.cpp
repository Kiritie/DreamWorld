// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/WidgetInventoryBar.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/SizeBox.h"
#include "Common/CommonBPLibrary.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Playing.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/Item/WidgetItemInfoBox.h"
#include "Widget/Inventory/Slot/WidgetInventoryAuxiliarySlot.h"
#include "Widget/Inventory/Slot/WidgetInventoryShortcutSlot.h"
#include "Widget/Inventory/Slot/WidgetInventorySkillSlot.h"

UWidgetInventoryBar::UWidgetInventoryBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InventoryBar");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::GameOnly;
	
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;

	WidgetZOrder = 1;

	SelectedSlotIndex = 0;

	ShortcutSlotClass = LoadClass<UWidgetInventoryShortcutSlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/WB_InventoryShortcutSlot.WB_InventoryShortcutSlot_C'"));
	AuxiliarySlotClass = LoadClass<UWidgetInventoryAuxiliarySlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/WB_InventoryAuxiliarySlot.WB_InventoryAuxiliarySlot_C'"));
	SkillSlotClass = LoadClass<UWidgetInventorySkillSlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/WB_InventorySkillSlot.WB_InventorySkillSlot_C'"));

	UISlotDatas.Add(ESlotSplitType::Shortcut);
	UISlotDatas.Add(ESlotSplitType::Auxiliary);
	UISlotDatas.Add(ESlotSplitType::Skill);
}

void UWidgetInventoryBar::OnInitialize_Implementation(UObject* InOwner)
{
	if(GetInventory())
	{
		GetInventory()->OnSlotSelected.RemoveDynamic(this, &UWidgetInventoryBar::OnInventorySlotSelected);
	}

	Super::OnInitialize_Implementation(InOwner);
	
	if(ShortcutContent && UISlotDatas.Contains(ESlotSplitType::Shortcut))
	{
		const auto ShortcutSlots = GetInventory()->GetSlotsBySplitType(ESlotSplitType::Shortcut);
		if(UISlotDatas[ESlotSplitType::Shortcut].Slots.Num() != ShortcutSlots.Num())
		{
			ShortcutContent->ClearChildren();
			UISlotDatas[ESlotSplitType::Shortcut].Slots.Empty();
			for(int32 i = 0; i < ShortcutSlots.Num(); i++)
			{
				if(UWidgetInventoryShortcutSlot* ShortcutSlot = CreateSubWidget<UWidgetInventoryShortcutSlot>({ ShortcutSlots[i] }, ShortcutSlotClass))
				{
					//ShortcutSlot->SetKeyCode(UDWHelper::GetInputActionKeyCodeByName(FString::Printf(TEXT("SelectInventorySlot%d"), i + 1)));
					ShortcutSlot->SetKeyCode(FText::FromString(FString::FromInt(i < 9 ? i + 1 : 0)));
					if(UGridSlot* GridSlot = ShortcutContent->AddChildToGrid(ShortcutSlot))
					{
						GridSlot->SetPadding(FMargin(0.f, 0.f, 5.f, 0.f));
						GridSlot->SetColumn(i);
					}
					UISlotDatas[ESlotSplitType::Shortcut].Slots.Add(ShortcutSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESlotSplitType::Shortcut].Slots.Num(); i++)
			{
				UISlotDatas[ESlotSplitType::Shortcut].Slots[i]->OnInitialize({ ShortcutSlots[i] });
			}
		}
	}
	if(AuxiliaryContent && UISlotDatas.Contains(ESlotSplitType::Auxiliary))
	{
		const auto AuxiliarySlots = GetInventory()->GetSlotsBySplitType(ESlotSplitType::Auxiliary);
		if(UISlotDatas[ESlotSplitType::Auxiliary].Slots.Num() == 0)
		{
			for(int32 i = 0; i < AuxiliarySlots.Num(); i++)
			{
				if(UWidgetInventoryAuxiliarySlot* AuxiliarySlot = CreateSubWidget<UWidgetInventoryAuxiliarySlot>({ AuxiliarySlots[i] }, AuxiliarySlotClass))
				{
					//AuxiliarySlot->SetKeyCode(UCommonBPLibrary::GetInputActionKeyCodeByName(FString::Printf(TEXT("ReleaseAuxiliaryAbility%d"), i + 1)));
					AuxiliarySlot->SetKeyCode(FText::FromString(TEXT("X")));
					if(UGridSlot* GridSlot = AuxiliaryContent->AddChildToGrid(AuxiliarySlot))
					{
						GridSlot->SetPadding(FMargin(0.f, 0.f, 5.f, 0.f));
						GridSlot->SetColumn(i);
					}
					UISlotDatas[ESlotSplitType::Auxiliary].Slots.Add(AuxiliarySlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESlotSplitType::Auxiliary].Slots.Num(); i++)
			{
				UISlotDatas[ESlotSplitType::Auxiliary].Slots[i]->OnInitialize({ AuxiliarySlots[i] });
			}
		}
	}
	if(LeftSkillContent && RightSkillContent && UISlotDatas.Contains(ESlotSplitType::Skill))
	{
		const auto SkillSlots = GetInventory()->GetSlotsBySplitType(ESlotSplitType::Skill);
		if(UISlotDatas[ESlotSplitType::Skill].Slots.Num() == 0)
		{
			for(int32 i = 0; i < SkillSlots.Num(); i++)
			{
				if(UWidgetInventorySkillSlot* SkillSlot = CreateSubWidget<UWidgetInventorySkillSlot>({ SkillSlots[i] }, SkillSlotClass))
				{
					SkillSlot->SetKeyCode(UCommonBPLibrary::GetInputActionKeyCodeByName(FString::Printf(TEXT("ReleaseSkillAbility%d"), i + 1)));
					if(UGridSlot* GridSlot = i < SkillSlots.Num() / 2 ? LeftSkillContent->AddChildToGrid(SkillSlot) : RightSkillContent->AddChildToGrid(SkillSlot))
					{
						GridSlot->SetPadding(FMargin(0.f, 0.f, 5.f, 0.f));
						GridSlot->SetColumn(i % (SkillSlots.Num() / 2));
					}
					UISlotDatas[ESlotSplitType::Skill].Slots.Add(SkillSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESlotSplitType::Skill].Slots.Num(); i++)
			{
				UISlotDatas[ESlotSplitType::Skill].Slots[i]->OnInitialize({ SkillSlots[i] });
			}
		}
	}

	GetInventory()->OnSlotSelected.AddDynamic(this, &UWidgetInventoryBar::OnInventorySlotSelected);

	if(GetInventory()->GetSelectedSlot())
	{
		SelectInventorySlot(GetInventory()->GetSelectedSlot()->GetSplitIndex(ESlotSplitType::Shortcut), false);
	}
	else
	{
		SelectInventorySlot(0);
	}
}

void UWidgetInventoryBar::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetInventoryBar::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

void UWidgetInventoryBar::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}

void UWidgetInventoryBar::OnInventorySlotSelected(UAbilityInventorySlot* InInventorySlot)
{
	if(InInventorySlot)
	{
		SelectInventorySlot(InInventorySlot->GetSplitIndex(ESlotSplitType::Shortcut), false);
	}
}

void UWidgetInventoryBar::PrevInventorySlot()
{
	if(SelectedSlotIndex > 0)
		SelectInventorySlot(SelectedSlotIndex - 1);
	else
		SelectInventorySlot(9);
}

void UWidgetInventoryBar::NextInventorySlot()
{
	if (SelectedSlotIndex < 9)
		SelectInventorySlot(SelectedSlotIndex + 1);
	else
		SelectInventorySlot(0);
}

void UWidgetInventoryBar::SelectInventorySlot(int32 InSlotIndex, bool bRefreshInventory)
{
	SelectedSlotIndex = InSlotIndex;
	if(UAbilityInventorySlot* SelectedSlot = GetSelectedSlot())
	{
		if(bRefreshInventory)
		{
			GetInventory()->SetSelectedSlot(SelectedSlot);
		}
		if(!SelectedSlot->IsEmpty() && UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
		{
			UWidgetModuleBPLibrary::OpenUserWidget<UWidgetItemInfoBox>({ SelectedSlot->GetItem().GetData().Name });
		}
	}
	if(SelectBox)
	{
		if(UCanvasPanelSlot* SelectBoxSlot = Cast<UCanvasPanelSlot>(SelectBox->Slot))
		{
			SelectBoxSlot->SetPosition(FVector2D(SelectedSlotIndex * 85.f, 0));
		}
	}
}

void UWidgetInventoryBar::SetSkillBoxVisible(bool bValue)
{
	if(LeftSkillBox)
	{
		LeftSkillBox->SetVisibility(bValue ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
	if(RightSkillBox)
	{
		RightSkillBox->SetVisibility(bValue ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

UAbilityInventorySlot* UWidgetInventoryBar::GetSelectedSlot() const
{
	auto SplitUISlots = GetSplitUISlots(ESlotSplitType::Shortcut);
	if(SplitUISlots.IsValidIndex(SelectedSlotIndex))
	{
		return SplitUISlots[SelectedSlotIndex]->GetOwnerSlot();
	}
	return nullptr;
}

FAbilityItem UWidgetInventoryBar::GetSelectedItem() const
{
	if (GetSelectedSlot())
	{
		return GetSelectedSlot()->GetItem();
	}
	return FAbilityItem::Empty;
}
