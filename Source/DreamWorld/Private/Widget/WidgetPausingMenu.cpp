// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetPausingMenu.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Pausing.h"
#include "Procedure/Procedure_Playing.h"

UWidgetPausingMenu::UWidgetPausingMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("PausingMenu");
	WidgetType = EWidgetType::Temporary;
	InputMode = EInputMode::UIOnly;

	bIsFocusable = true;
}

void UWidgetPausingMenu::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetPausingMenu::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);
}

void UWidgetPausingMenu::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
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
