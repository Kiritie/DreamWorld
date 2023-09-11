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
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Widget/Inventory/Item/WidgetInventoryItemInfoBox.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Global/GlobalBPLibrary.h"
#include "Widget/Inventory/Item/WidgetInventoryItem.h"

UWidgetInventorySlot::UWidgetInventorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UWidgetInventoryItemBase> ItemDragVisualClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Item/WB_InventoryItemDragVisual.WB_InventoryItemDragVisual_C'"));
	if(ItemDragVisualClassFinder.Succeeded())
	{
		DragVisualClass = ItemDragVisualClassFinder.Class;
	}
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
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryItemInfoBox>({ ItemData.Name, ItemData.Detail });
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
		if(const auto ItemInfoBoxSlot = Cast<UCanvasPanelSlot>(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryItemInfoBox>()->Slot))
		{
			float PosX, PosY;
			UWidgetLayoutLibrary::GetMousePositionScaledByDPI(UGlobalBPLibrary::GetPlayerController(), PosX, PosY);
			ItemInfoBoxSlot->SetPosition(FVector2D(PosX, PosY));
		}
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UWidgetInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, FKey("LeftMouseButton")).NativeReply;
}

void UWidgetInventorySlot::OnCreate_Implementation(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate_Implementation(InOwner, InParams);
}

void UWidgetInventorySlot::OnInitialize_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnInitialize_Implementation(InParams);
}

void UWidgetInventorySlot::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

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
			UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryItemInfoBox>({ ItemData.Name, ItemData.Detail });
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
