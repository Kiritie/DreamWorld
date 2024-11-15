// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskRootItem.h"

#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Widget/Task/WidgetTaskCategory.h"
#include "Widget/Task/WidgetTaskItem.h"
#include "Widget/Task/WidgetTaskPanel.h"

UWidgetTaskRootItem::UWidgetTaskRootItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = true;
	bToggleable = true;

	TxtName = nullptr;
	TxtDetail = nullptr;

	Task = nullptr;
	TaskContent = nullptr;
	TaskCategory = nullptr;
}

void UWidgetTaskRootItem::OnDespawn_Implementation(bool bRecovery)
{
	Task = nullptr;
	TaskContent = nullptr;
	TaskCategory = nullptr;
	
	for(auto Iter : TaskItems)
	{
		OwnerWidget->DestroySubWidget(Iter, true);
	}
	TaskItems.Empty();

	Super::OnDespawn_Implementation(bRecovery);
}

void UWidgetTaskRootItem::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		Task = InParams[0];
	}
	if(InParams.IsValidIndex(1))
	{
		TaskContent = InParams[1];
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
}

void UWidgetTaskRootItem::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);

	if(UWidgetTaskPanel* TaskPanel = GetOwnerWidget<UWidgetTaskPanel>())
	{
		for(auto Iter : Task->SubTasks)
		{
			if(UWidgetTaskItem* TaskItem = TaskPanel->CreateSubWidget<UWidgetTaskItem>({ Iter }, TaskPanel->TaskItemClass))
			{
				TaskItems.Add(TaskItem);
				if(UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(TaskContent->AddChild(TaskItem)))
				{
					VerticalBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				}
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
		for(auto Iter : TaskItems)
		{
			OwnerWidget->DestroySubWidget(Iter, true);
		}
		TaskItems.Empty();
		
		TaskPanel->OnTaskRootItemDeselected(this);
	}
}
