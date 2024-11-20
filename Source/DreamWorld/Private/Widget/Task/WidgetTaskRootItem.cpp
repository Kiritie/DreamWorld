// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskRootItem.h"

#include "Components/TextBlock.h"
#include "Widget/Task/WidgetTaskCategory.h"
#include "Widget/Task/WidgetTaskContainer.h"
#include "Widget/Task/WidgetTaskItem.h"
#include "Widget/Task/WidgetTaskPanel.h"

UWidgetTaskRootItem::UWidgetTaskRootItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = true;
	bToggleable = true;

	TxtName = nullptr;
	TxtDetail = nullptr;

	Task = nullptr;
	TaskContainer = nullptr;
	TaskCategory = nullptr;
}

void UWidgetTaskRootItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	Task = nullptr;
	TaskContainer->Destroy(true);
	TaskContainer = nullptr;
	TaskCategory = nullptr;
}

void UWidgetTaskRootItem::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		Task = InParams[0];
	}
	if(InParams.IsValidIndex(1))
	{
		TaskContainer = InParams[1];
	}
	if(InParams.IsValidIndex(2))
	{
		TaskCategory = InParams[2];
	}
	Super::OnInitialize(InParams);
}

void UWidgetTaskRootItem::OnRefresh()
{
	Super::OnRefresh();

	if(Task)
	{
		TxtName->SetText(Task->TaskDisplayName);
		TxtDetail->SetText(Task->TaskDescription);
	}
	if(TaskContainer)
	{
		for(auto Iter : TaskContainer->GetTaskItems())
		{
			Iter->Refresh();
		}
	}
}

void UWidgetTaskRootItem::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);

	if(UWidgetTaskPanel* TaskPanel = GetOwnerWidget<UWidgetTaskPanel>())
	{
		TaskContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		for(auto Iter : Task->SubTasks)
		{
			if(UWidgetTaskItem* TaskItem = TaskPanel->CreateSubWidget<UWidgetTaskItem>({ Iter }, TaskPanel->TaskItemClass))
			{
				TaskContainer->AddTaskItem(TaskItem);
			}
		}
		TaskPanel->OnTaskRootItemSelected(this);
	}
}

void UWidgetTaskRootItem::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);

	if(UWidgetTaskPanel* TaskPanel = GetOwnerWidget<UWidgetTaskPanel>())
	{
		TaskContainer->SetVisibility(ESlateVisibility::Collapsed);
		TaskContainer->ClearTaskItem();
		TaskPanel->OnTaskRootItemDeselected(this);
	}
}
