// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/WidgetInventorySlot.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Item/WidgetAbilityItemInfoBox.h"

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
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
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

void UWidgetInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

void UWidgetInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	SetBorderColor(FLinearColor(1.0f, 0.843f, 0.0f, 1.0f));

	if(!IsEmpty())
	{
		const auto& ItemData = GetItem().GetData();
		UWidgetModuleStatics::OpenUserWidget<UWidgetAbilityItemInfoBox>({ ItemData.Name, ItemData.Detail });
	}
}

void UWidgetInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetBorderColor(FLinearColor(0.0f, 1.0f, 1.0f, 0.7f));

	if(!IsEmpty())
	{
		UWidgetModuleStatics::CloseUserWidget<UWidgetAbilityItemInfoBox>();
	}
}

FReply UWidgetInventorySlot::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(!IsEmpty())
	{
		if(const UWidgetAbilityItemInfoBox* ItemInfoBox = UWidgetModuleStatics::GetUserWidget<UWidgetAbilityItemInfoBox>())
		{
			if(const auto ItemInfoBoxSlot = Cast<UCanvasPanelSlot>(ItemInfoBox->Slot))
			{
				ItemInfoBoxSlot->SetPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
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
		return FReply::Handled();
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
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UWidgetInventorySlot::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetInventorySlot::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetInventorySlot::OnRefresh()
{
	Super::OnRefresh();

	if(!OwnerSlot) return;

	if(!IsEmpty())
	{
		const auto& ItemData = GetItem().GetData();
		ImgIcon->SetVisibility(ESlateVisibility::Visible);
		ImgIcon->SetBrushResourceObject(ItemData.Icon);
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
			UWidgetModuleStatics::OpenUserWidget<UWidgetAbilityItemInfoBox>({ ItemData.Name, ItemData.Detail });
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
			UWidgetModuleStatics::CloseUserWidget<UWidgetAbilityItemInfoBox>();
		}
	}
}

void UWidgetInventorySlot::OnActivated_Implementation()
{
	Super::OnActivated_Implementation();
}

void UWidgetInventorySlot::OnCanceled_Implementation()
{
	Super::OnCanceled_Implementation();
}

void UWidgetInventorySlot::StartCooldown_Implementation()
{
	Super::StartCooldown_Implementation();
}

void UWidgetInventorySlot::StopCooldown_Implementation()
{
	Super::StopCooldown_Implementation();

	ImgMask->SetVisibility(ESlateVisibility::Hidden);
	TxtCooldown->SetVisibility(ESlateVisibility::Hidden);
}

void UWidgetInventorySlot::OnCooldown_Implementation()
{
	if(!OwnerSlot) return;

	const FAbilityInfo CooldownInfo = OwnerSlot->GetAbilityInfo();
	if(CooldownInfo.IsCooldownning())
	{
		ImgMask->SetVisibility(ESlateVisibility::Visible);
		TxtCooldown->SetVisibility(ESlateVisibility::Visible);
		TxtCooldown->SetText(UKismetTextLibrary::Conv_DoubleToText(CooldownInfo.CooldownRemaining, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 1));
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
