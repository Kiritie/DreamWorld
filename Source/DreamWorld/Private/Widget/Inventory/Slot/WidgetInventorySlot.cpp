// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/WidgetInventorySlot.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Input/Widget/WidgetKeyTipsItemBase.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Item/Info/WidgetAbilityItemInfoBox.h"

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
}

void UWidgetInventorySlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

void UWidgetInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

void UWidgetInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if(!IsEmpty())
	{
		UWidgetModuleStatics::OpenUserWidget<UWidgetAbilityItemInfoBox>({ &GetMatchItems() });
	}
}

void UWidgetInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

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
	if(InMouseEvent.GetEffectingButton() == FKey("LeftMouseButton"))
	{
		if(InMouseEvent.IsLeftShiftDown())
		{
			MoveItem(-1);
			return FReply::Handled();
		}
		if(InMouseEvent.IsLeftControlDown())
		{
			SplitItem(2);
			return FReply::Handled();
		}
	}
	else if(InMouseEvent.GetEffectingButton() == FKey("RightMouseButton"))
	{
		if(InMouseEvent.IsLeftShiftDown())
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

void UWidgetInventorySlot::OnTick_Implementation(float DeltaSeconds)
{
	Super::OnTick_Implementation(DeltaSeconds);

	if(!IsEmpty())
	{
		const auto& ItemData = GetItem().GetData();
		if(ImgError)
		{
			ImgError->SetVisibility(ItemData.GetItemErrorInfo(GetOwnerWidget<UUserWidgetBase>()->GetOwnerObject<AActor>()).IsEmpty() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		}
	}
}

void UWidgetInventorySlot::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetInventorySlot::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);

	if(InParams.IsValidIndex(1))
	{
		if(KeyTipsItem)
		{
			KeyTipsItem->SetKeyMappingName(InParams[1]);
		}
	}
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
			UWidgetModuleStatics::OpenUserWidget<UWidgetAbilityItemInfoBox>({ &GetMatchItems() });
		}
	}
	else
	{
		ImgIcon->SetVisibility(ESlateVisibility::Hidden);
		if(ImgError)
		{
			ImgError->SetVisibility(ESlateVisibility::Hidden);
		}
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

void UWidgetInventorySlot::OnDeactivated_Implementation()
{
	Super::OnDeactivated_Implementation();
}

void UWidgetInventorySlot::StartCooldown_Implementation()
{
	Super::StartCooldown_Implementation();
}

void UWidgetInventorySlot::StopCooldown_Implementation()
{
	Super::StopCooldown_Implementation();

	if(ImgMask)
	{
		ImgMask->SetVisibility(ESlateVisibility::Hidden);
	}
	if(TxtCooldown)
	{
		TxtCooldown->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWidgetInventorySlot::OnCooldown_Implementation()
{
	if(!OwnerSlot) return;

	const FAbilityInfo AbilityInfo = OwnerSlot->GetAbilityInfo();
	if(AbilityInfo.IsCooldownning())
	{
		if(ImgMask)
		{
			ImgMask->SetVisibility(ESlateVisibility::Visible);
		}
		if(TxtCooldown)
		{
			TxtCooldown->SetVisibility(ESlateVisibility::Visible);
			TxtCooldown->SetText(UKismetTextLibrary::Conv_DoubleToText(AbilityInfo.CooldownRemaining, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 1));
		}
		if(MaskMatInst)
		{
			MaskMatInst->SetScalarParameterValue(FName("Progress"), 1.f - AbilityInfo.CooldownRemaining / AbilityInfo.CooldownDuration);
		}
	}
	else
	{
		StopCooldown();
	}
}
