// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/DWProcedure_Pausing.h"

#include "Common/CommonStatics.h"
#include "Procedure/DWProcedure_Starting.h"
#include "Widget/Main/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Menu/WidgetPausingMenu.h"

UDWProcedure_Pausing::UDWProcedure_Pausing()
{
	ProcedureName = FName("Pausing");
	ProcedureDisplayName = FText::FromString(TEXT("暂停"));
}

#if WITH_EDITOR
void UDWProcedure_Pausing::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_Pausing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_Pausing::OnInitialize()
{
	Super::OnInitialize();
}

void UDWProcedure_Pausing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	UCommonStatics::PauseGame();

	UWidgetModuleStatics::OpenUserWidget<UWidgetPausingMenu>();
}

void UDWProcedure_Pausing::OnRefresh()
{
	Super::OnRefresh();
}

void UDWProcedure_Pausing::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_Pausing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);

	UCommonStatics::UnPauseGame();

	if(InNextProcedure->IsA<UDWProcedure_Starting>())
	{
		UWidgetModuleStatics::CloseUserWidget<UWidgetGameHUD>();
		UWidgetModuleStatics::CloseUserWidget<UWidgetInventoryBar>();
	}

	UWidgetModuleStatics::CloseUserWidget<UWidgetPausingMenu>();
}
