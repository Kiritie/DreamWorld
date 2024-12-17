// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Archive/WidgetArchiveCreatingPanel.h"

#include "CommonAnimatedSwitcher.h"
#include "Widget/Archive/WidgetArchiveCreatingPage.h"

UWidgetArchiveCreatingPanel::UWidgetArchiveCreatingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ArchiveCreatingPanel");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::GameAndUI_NotHideCursor;
}

void UWidgetArchiveCreatingPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetArchiveCreatingPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetArchiveCreatingPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

void UWidgetArchiveCreatingPanel::OnRefresh()
{
	Super::OnRefresh();
}

UWidgetArchiveCreatingPage* UWidgetArchiveCreatingPanel::GetCurrentPage() const
{
	return GetSubWidget<UWidgetArchiveCreatingPage>(CurrentPageIndex);
}

void UWidgetArchiveCreatingPanel::SetCurrentPage_Implementation(int32 InPageIndex)
{
	CurrentPageIndex = InPageIndex;
	Switcher_Page->SetActiveWidgetIndex(InPageIndex);
}
