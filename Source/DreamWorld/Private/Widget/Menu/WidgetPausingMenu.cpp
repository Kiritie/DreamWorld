// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Menu/WidgetPausingMenu.h"

#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Pausing.h"
#include "Procedure/Procedure_Playing.h"

UWidgetPausingMenu::UWidgetPausingMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("PausingMenu");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;

	WidgetZOrder = 10;

	SetIsFocusable(true);
}

void UWidgetPausingMenu::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetPausingMenu::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetPausingMenu::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

FReply UWidgetPausingMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == FKey("Escape"))
	{
		if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Pausing>())
		{
			UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Playing>();
			return FReply::Handled();
		}
	}
	return FReply::Unhandled();
}
