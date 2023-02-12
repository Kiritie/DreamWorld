// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/WidgetInventorySlot.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Ability/Vitality/AbilityVitalityInterface.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Ability/Inventory/Inventory.h"
#include "Widget/Inventory/WidgetInventory.h"
#include "Ability/Inventory/Slot/InventorySlot.h"
#include "Kismet/KismetTextLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Widget/Inventory/WidgetInventoryItemInfoBox.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Global/GlobalBPLibrary.h"

UWidgetInventorySlot::UWidgetInventorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(ImgMask)
	{
		MaskMatInst = ImgMask->GetDynamicMaterial();
		MaskMatInst->SetVectorParameterValue(FName("Color"), FLinearColor(0.f, 0.f, 0.f, 0.3f));
		MaskMatInst->SetScalarParameterValue(FName("Progress"), 0.5f);
	}
}

void UWidgetInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UWidgetInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	auto payloadSlot = Cast<UWidgetInventorySlot>(InOperation->Payload);
	if (payloadSlot && payloadSlot != this && !payloadSlot->IsEmpty())
	{
		FAbilityItem& tmpItem = payloadSlot->GetItem();
		if(OwnerSlot->CheckSlot(tmpItem))
		{
			if (OwnerSlot->Contains(tmpItem))
			{
				OwnerSlot->AddItem(tmpItem);
				payloadSlot->OwnerSlot->Refresh();
			}
			else
			{
				OwnerSlot->Replace(payloadSlot->OwnerSlot);
			}
		}
	}
	return true;
}

void UWidgetInventorySlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	SetBorderColor(FLinearColor(1.0f, 0.843f, 0.0f, 1.0f));
}

void UWidgetInventorySlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	SetBorderColor(FLinearColor(0.0f, 1.0f, 1.0f, 0.7f));
}

void UWidgetInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	SetBorderColor(FLinearColor(1.0f, 0.843f, 0.0f, 1.0f));

	if(!IsEmpty())
	{
		const auto& ItemData = GetItem().GetData();
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryItemInfoBox>({ FParameter::MakeText(ItemData.Name), FParameter::MakeText(ItemData.Detail) });
	}
}

void UWidgetInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetBorderColor(FLinearColor(0.0f, 1.0f, 1.0f, 0.7f));

	if(!IsEmpty())
	{
		UWidgetModuleBPLibrary::CloseUserWidget<UWidgetInventoryItemInfoBox>();
	}
}

FReply UWidgetInventorySlot::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(!IsEmpty())
	{
		if(auto InventoryItemInfoBox = UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryItemInfoBox>())
		{
			if(auto InventoryItemInfoBoxSlot = Cast<UCanvasPanelSlot>(InventoryItemInfoBox->Slot))
			{
				float PosX, PosY;
				UWidgetLayoutLibrary::GetMousePositionScaledByDPI(UGlobalBPLibrary::GetPlayerController(), PosX, PosY);
				InventoryItemInfoBoxSlot->SetPosition(FVector2D(PosX, PosY));
			}
		}
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UWidgetInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.GetEffectingButton() == FKey("RightMouseButton"))
	{
		if(InMouseEvent.IsLeftControlDown())
		{
			MoveItem(-1);
		}
		else if(InMouseEvent.IsLeftShiftDown())
		{
			UseItem(-1);
		}
		else
		{
			UseItem(1);
		}
	}
	else if(InMouseEvent.GetEffectingButton() == FKey("MiddleMouseButton"))
	{
		if(InMouseEvent.IsLeftShiftDown())
		{
			DiscardItem(-1);
		}
		else
		{
			DiscardItem(1);
		}
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UWidgetInventorySlot::OnInitialize(UInventorySlot* InOwnerSlot)
{
	Super::OnInitialize(InOwnerSlot);
}

void UWidgetInventorySlot::OnRefresh()
{
	Super::OnRefresh();

	if(!OwnerSlot) return;

	if(!IsEmpty())
	{
		ImgIcon->SetVisibility(ESlateVisibility::Visible);
		const auto& ItemData = GetItem().GetData();
		if(ItemData.IconMat)
		{
			ImgIcon->SetBrushFromMaterial(ItemData.IconMat);
		}
		else
		{
			ImgIcon->SetBrushFromTexture(ItemData.Icon);
		}
		if(TxtCount)
		{
			if(GetItem().Count > 1)
			{
				TxtCount->SetVisibility(ESlateVisibility::Visible);
				TxtCount->SetText(FText::FromString(FString::FromInt(GetItem().Count)));
			}
			else
			{
				TxtCount->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		if(IsHovered())
		{
			UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryItemInfoBox>({ FParameter::MakeText(ItemData.Name), FParameter::MakeText(ItemData.Detail) });
		}
	}
	else
	{
		ImgIcon->SetVisibility(ESlateVisibility::Hidden);
		if(TxtCount)
		{
			TxtCount->SetVisibility(ESlateVisibility::Hidden);
		}
		if(IsHovered())
		{
			UWidgetModuleBPLibrary::CloseUserWidget<UWidgetInventoryItemInfoBox>();
		}
	}
}

void UWidgetInventorySlot::OnActivated()
{
	Super::OnActivated();
}

void UWidgetInventorySlot::OnCanceled()
{
	Super::OnCanceled();
}

void UWidgetInventorySlot::StartCooldown()
{
	Super::StartCooldown();
}

void UWidgetInventorySlot::StopCooldown()
{
	Super::StopCooldown();

	ImgMask->SetVisibility(ESlateVisibility::Hidden);
	TxtCooldown->SetVisibility(ESlateVisibility::Hidden);
}

void UWidgetInventorySlot::OnCooldown()
{
	if(!OwnerSlot) return;

	const FAbilityInfo CooldownInfo = OwnerSlot->GetAbilityInfo();
	if(CooldownInfo.CooldownRemaining > 0.f)
	{
		ImgMask->SetVisibility(ESlateVisibility::Visible);
		TxtCooldown->SetVisibility(ESlateVisibility::Visible);
		TxtCooldown->SetText(UKismetTextLibrary::AsCurrency_Float(CooldownInfo.CooldownRemaining, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 1));
		MaskMatInst->SetScalarParameterValue(FName("Progress"), 1.f - CooldownInfo.CooldownRemaining / CooldownInfo.CooldownDuration);
	}
	else
	{
		StopCooldown();
	}
}

void UWidgetInventorySlot::SetBorderColor(FLinearColor InColor)
{
	Border->SetBrushColor(InColor);
}
