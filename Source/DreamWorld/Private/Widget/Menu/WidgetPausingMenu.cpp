// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Menu/WidgetPausingMenu.h"

#include "Input/InputModuleStatics.h"
#include "Procedure/ProcedureModuleStatics.h"
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

void UWidgetPausingMenu::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
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
	if(UInputModuleStatics::IsPlayerMappedKeyByName(FName("SystemOperation"), InKeyEvent.GetKey()))
	{
		if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Pausing>())
		{
			UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_Playing>();
			return FReply::Handled();
		}
	}
	return FReply::Unhandled();
}
