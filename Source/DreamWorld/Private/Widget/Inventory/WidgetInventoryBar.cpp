// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/WidgetInventoryBar.h"

#include "Asset/Primary/Item/ItemAssetBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "GameFramework/InputSettings.h"
#include "Inventory/Inventory.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetItemInfoBox.h"
#include "Widget/Inventory/Slot/WidgetInventoryAuxiliarySlot.h"
#include "Widget/Inventory/Slot/WidgetInventoryShortcutSlot.h"
#include "Widget/Inventory/Slot/WidgetInventorySkillSlot.h"

UWidgetInventoryBar::UWidgetInventoryBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InventoryBar");
	WidgetType = EWidgetType::Permanent;
	InputMode = EInputMode::GameOnly;
	
	SelectedSlotIndex = 0;

	ShortcutSlotClass = LoadClass<UWidgetInventoryShortcutSlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/WB_InventoryShortcutSlot.WB_InventoryShortcutSlot_C'"));
	AuxiliarySlotClass = LoadClass<UWidgetInventoryAuxiliarySlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/WB_InventoryAuxiliarySlot.WB_InventoryAuxiliarySlot_C'"));
	SkillSlotClass = LoadClass<UWidgetInventorySkillSlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/WB_InventorySkillSlot.WB_InventorySkillSlot_C'"));

	UISlotDatas.Add(ESplitSlotType::Shortcut);
	UISlotDatas.Add(ESplitSlotType::Auxiliary);
	UISlotDatas.Add(ESplitSlotType::Skill);
}

void UWidgetInventoryBar::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);

	if(!GetInventory()) return;
	
	if(ShortcutContent && UISlotDatas.Contains(ESplitSlotType::Shortcut))
	{
		const auto ShortcutSlots = GetInventory()->GetSplitSlots<UInventorySlot>(ESplitSlotType::Shortcut);
		if(UISlotDatas[ESplitSlotType::Shortcut].Slots.Num() != ShortcutSlots.Num())
		{
			ShortcutContent->ClearChildren();
			UISlotDatas[ESplitSlotType::Shortcut].Slots.Empty();
			for(int32 i = 0; i < ShortcutSlots.Num(); i++)
			{
				if(UWidgetInventoryShortcutSlot* ShortcutSlot = Cast<UWidgetInventoryShortcutSlot>(UWidgetBlueprintLibrary::Create(this, ShortcutSlotClass, nullptr)))
				{
					ShortcutSlot->InitSlot(ShortcutSlots[i]);
					//ShortcutSlot->SetKeyCode(UDWHelper::GetInputActionKeyCodeByName(FString::Printf(TEXT("SelectInventorySlot%d"), i + 1)));
					ShortcutSlot->SetKeyCode(FText::FromString(FString::FromInt(i < 9 ? i + 1 : 0)));
					if(UGridSlot* GridSlot = ShortcutContent->AddChildToGrid(ShortcutSlot))
					{
						GridSlot->SetPadding(FMargin(0.f, 0.f, 5.f, 0.f));
						GridSlot->SetColumn(i);
					}
					UISlotDatas[ESplitSlotType::Shortcut].Slots.Add(ShortcutSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESplitSlotType::Shortcut].Slots.Num(); i++)
			{
				UISlotDatas[ESplitSlotType::Shortcut].Slots[i]->InitSlot(ShortcutSlots[i]);
			}
		}
	}
	if(AuxiliaryContent && UISlotDatas.Contains(ESplitSlotType::Auxiliary))
	{
		const auto AuxiliarySlots = GetInventory()->GetSplitSlots<UInventorySlot>(ESplitSlotType::Auxiliary);
		if(UISlotDatas[ESplitSlotType::Auxiliary].Slots.Num() == 0)
		{
			for(int32 i = 0; i < AuxiliarySlots.Num(); i++)
			{
				if(UWidgetInventoryAuxiliarySlot* AuxiliarySlot = Cast<UWidgetInventoryAuxiliarySlot>(UWidgetBlueprintLibrary::Create(this, AuxiliarySlotClass, nullptr)))
				{
					AuxiliarySlot->InitSlot(AuxiliarySlots[i]);
					AuxiliarySlot->SetKeyCode(UGlobalBPLibrary::GetInputActionKeyCodeByName(FString::Printf(TEXT("ReleaseAuxiliaryAbility%d"), i + 1)));
					if(UGridSlot* GridSlot = AuxiliaryContent->AddChildToGrid(AuxiliarySlot))
					{
						GridSlot->SetPadding(FMargin(0.f, 0.f, 5.f, 0.f));
						GridSlot->SetColumn(i);
					}
					UISlotDatas[ESplitSlotType::Auxiliary].Slots.Add(AuxiliarySlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESplitSlotType::Auxiliary].Slots.Num(); i++)
			{
				UISlotDatas[ESplitSlotType::Auxiliary].Slots[i]->InitSlot(AuxiliarySlots[i]);
			}
		}
	}
	if(LeftSkillContent && RightSkillContent && UISlotDatas.Contains(ESplitSlotType::Skill))
	{
		const auto SkillSlots = GetInventory()->GetSplitSlots<UInventorySlot>(ESplitSlotType::Skill);
		if(UISlotDatas[ESplitSlotType::Skill].Slots.Num() == 0)
		{
			for(int32 i = 0; i < SkillSlots.Num(); i++)
			{
				if(UWidgetInventorySkillSlot* SkillSlot = Cast<UWidgetInventorySkillSlot>(UWidgetBlueprintLibrary::Create(this, SkillSlotClass, nullptr)))
				{
					SkillSlot->InitSlot(SkillSlots[i]);
					SkillSlot->SetKeyCode(UGlobalBPLibrary::GetInputActionKeyCodeByName(FString::Printf(TEXT("ReleaseSkillAbility%d"), i + 1)));
					if(UGridSlot* GridSlot = i < SkillSlots.Num() / 2 ? LeftSkillContent->AddChildToGrid(SkillSlot) : RightSkillContent->AddChildToGrid(SkillSlot))
					{
						GridSlot->SetPadding(FMargin(0.f, 0.f, 5.f, 0.f));
						GridSlot->SetColumn(i % (SkillSlots.Num() / 2));
					}
					UISlotDatas[ESplitSlotType::Skill].Slots.Add(SkillSlot);
				}
			}
		}
		else
		{
			for(int32 i = 0; i < UISlotDatas[ESplitSlotType::Skill].Slots.Num(); i++)
			{
				UISlotDatas[ESplitSlotType::Skill].Slots[i]->InitSlot(SkillSlots[i]);
			}
		}
	}
}

void UWidgetInventoryBar::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);
}

void UWidgetInventoryBar::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
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

void UWidgetInventoryBar::SelectInventorySlot(int32 InSlotIndex)
{
	SelectedSlotIndex = InSlotIndex;
	UpdateSelectBox();
	GetInventory()->SetSelectedSlot(GetSelectedSlot());
	TArray<FParameter> Params { FParameter::MakeString(GetSelectedItem().GetData()->Name.ToString()) };
	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetItemInfoBox>(&Params);
	UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->RefreshActions();
}

UInventorySlot* UWidgetInventoryBar::GetSelectedSlot() const
{
	auto SplitUISlots = GetSplitUISlots(ESplitSlotType::Shortcut);
	if(SplitUISlots.Num() > SelectedSlotIndex)
	{
		return SplitUISlots[SelectedSlotIndex]->GetOwnerSlot();
	}
	return nullptr;
}

FItem UWidgetInventoryBar::GetSelectedItem() const
{
	if (GetSelectedSlot())
	{
		return GetSelectedSlot()->GetItem();
	}
	return FItem::Empty;
}
