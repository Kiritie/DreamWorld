// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskRootItem.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Task/Base/TaskAsset.h"
#include "Widget/Task/WidgetTaskContainer.h"
#include "Widget/Task/WidgetTaskItem.h"
#include "Widget/Task/WidgetTaskPanel.h"

UWidgetTaskRootItem::UWidgetTaskRootItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskContainerClass = nullptr;
	TaskAsset = nullptr;
	TaskContainer = nullptr;
}

void UWidgetTaskRootItem::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	TaskContainer = UObjectPoolModuleStatics::SpawnObject<UWidgetTaskContainer>(this, nullptr, TaskContainerClass);
	TaskContainer->SetVisibility(ESlateVisibility::Collapsed);

	Super::OnCreate(InOwner, InParams);
}

void UWidgetTaskRootItem::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(1))
	{
		TaskAsset = InParams[1];
	}
	Super::OnInitialize(InParams);
}

void UWidgetTaskRootItem::OnRefresh()
{
	Super::OnRefresh();

	for(auto Iter : TaskContainer->GetTaskItems())
	{
		Iter->Refresh();
	}
}

void UWidgetTaskRootItem::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);

	TaskAsset = nullptr;
	TaskContainer->Destroy(bRecovery);
	TaskContainer = nullptr;
}

void UWidgetTaskRootItem::NativeOnSelected(bool bBroadcast)
{
	USubButtonWidgetBase::NativeOnSelected(bBroadcast);

	if(bBroadcast)
	{
		if(UWidgetTaskPanel* TaskPanel = GetOwnerWidget<UWidgetTaskPanel>())
		{
			TaskContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			for(auto Iter : Task->SubTasks)
			{
				if(Iter->IsLeaved()) continue;
					
				if(UWidgetTaskItem* TaskItem = TaskPanel->CreateSubWidget<UWidgetTaskItem>({ Iter }, TaskPanel->TaskItemClass))
				{
					TaskContainer->AddTaskItem(TaskItem);
				}
			}
			TaskPanel->OnTaskRootItemSelected(this);
		}
	}
}

void UWidgetTaskRootItem::NativeOnDeselected(bool bBroadcast)
{
	USubButtonWidgetBase::NativeOnDeselected(bBroadcast);

	if(bBroadcast)
	{
		if(UWidgetTaskPanel* TaskPanel = GetOwnerWidget<UWidgetTaskPanel>())
		{
			TaskContainer->SetVisibility(ESlateVisibility::Collapsed);
			TaskContainer->ClearTaskItem();
			TaskPanel->OnTaskRootItemDeselected(this);
		}
	}
}

void UWidgetTaskRootItem::OnAddToContainer(UWidgetTaskContainer* InTaskContainer)
{
	Super::OnAddToContainer(InTaskContainer);

	if(UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(InTaskContainer->GetTaskContent()->AddChild(TaskContainer)))
	{
		VerticalBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
	}
}
