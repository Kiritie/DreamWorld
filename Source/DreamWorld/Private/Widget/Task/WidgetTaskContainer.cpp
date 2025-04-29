// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskContainer.h"

#include "Widget/Task/WidgetTaskItem.h"

UWidgetTaskContainer::UWidgetTaskContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TaskContent = nullptr;
}

void UWidgetTaskContainer::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	ClearTaskItem();
}

void UWidgetTaskContainer::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetTaskContainer::AddTaskItem(UWidgetTaskItem* InTaskItem)
{
	if(TaskItems.Contains(InTaskItem)) return;
	
	TaskItems.Add(InTaskItem);
	InTaskItem->OnAddToContainer(this);
}

void UWidgetTaskContainer::ClearTaskItem()
{
	for(auto Iter : TaskItems)
	{
		Iter->Destroy(true);
	}
	TaskItems.Empty();
}
