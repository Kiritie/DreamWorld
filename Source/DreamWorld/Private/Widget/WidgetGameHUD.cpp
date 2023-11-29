// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetGameHUD.h"

#include "Character/DWCharacter.h"
#include "Event/EventModuleStatics.h"
#include "Event/Handle/Input/EventHandle_ChangeInputMode.h"
#include "Widget/WidgetModule.h"


UWidgetGameHUD::UWidgetGameHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GameHUD");
	ChildNames =
	{
		FName("InventoryBar"),
	    FName("InventoryBox"),
	    FName("InventoryPanel"),
	    FName("GeneratePanel"),
	    FName("ShopPanel"),
	    FName("ContextBox"),
	    FName("MessageBox"),
	    FName("TaskInfoBox")
	};
	WidgetType = EWidgetType::Permanent;
	WidgetInputMode = EInputMode::GameOnly;

	WidgetRefreshType = EWidgetRefreshType::Tick;

	SetIsFocusable(true);
}

void UWidgetGameHUD::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	UEventModuleStatics::SubscribeEvent(UEventHandle_ChangeInputMode::StaticClass(), this, FName("OnChangeInputMode"));
}

void UWidgetGameHUD::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetGameHUD::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetGameHUD::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

void UWidgetGameHUD::OnRefresh()
{
	Super::OnRefresh();
}

FReply UWidgetGameHUD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == FKey("Escape"))
	{
		if(TemporaryChild)
		{
			TemporaryChild->Close();
			return FReply::Handled();
		}
	}
	return FReply::Unhandled();
}

void UWidgetGameHUD::OnChangeInputMode(UObject* InSender, UEventHandle_ChangeInputMode* InEventHandle)
{
	if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(OwnerObject))
	{
		if(!OwnerCharacter->IsDead() && InEventHandle->InputMode == EInputMode::GameOnly)
		{
			SetCrosshairVisible(true);
		}
		else
		{
			SetCrosshairVisible(false);
		}
	}
	else
	{
		SetCrosshairVisible(true);
	}
}
