// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskCategory.h"

#include "Components/TextBlock.h"
#include "Widget/Task/WidgetTaskContainer.h"
#include "Widget/Task/WidgetTaskItem.h"
#include "Widget/Task/WidgetTaskPanel.h"

UWidgetTaskCategory::UWidgetTaskCategory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TxtName = nullptr;

	TaskContainer = nullptr;
}

void UWidgetTaskCategory::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetTaskCategory::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		TaskName = InParams[0];
	}
	Super::OnInitialize(InParams);
}

void UWidgetTaskCategory::OnRefresh()
{
	Super::OnRefresh();

	if(TxtName)
	{
		TxtName->SetText(TaskName);
	}

	for(auto Iter : TaskContainer->GetTaskItems())
	{
		Iter->Refresh();
	}
}

void UWidgetTaskCategory::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);

	TaskName = FText::GetEmpty();
	TaskContainer->ClearTaskItem();
}
