// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Task/WidgetTaskInfoBox.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Event/EventModuleStatics.h"
#include "Event/Handle/Task/EventHandle_TaskCompleted.h"
#include "Event/Handle/Task/EventHandle_TaskEntered.h"
#include "Task/TaskModuleStatics.h"
#include "Task/Base/TaskBase.h"
#include "Widget/WidgetModule.h"
#include "Widget/Task/WidgetTaskInfoItem.h"


UWidgetTaskInfoBox::UWidgetTaskInfoBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("TaskInfoBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_TaskInfoBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetTaskInfoBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetTaskInfoBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetTaskInfoBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	UEventModuleStatics::SubscribeEvent<UEventHandle_TaskEntered>(this, GET_FUNCTION_NAME_THISCLASS(OnTaskInfoContentRefresh));

	UEventModuleStatics::SubscribeEvent<UEventHandle_TaskCompleted>(this, GET_FUNCTION_NAME_THISCLASS(Refresh));
}

void UWidgetTaskInfoBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_TaskEntered>(this, GET_FUNCTION_NAME_THISCLASS(OnTaskInfoContentRefresh));

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_TaskCompleted>(this, GET_FUNCTION_NAME_THISCLASS(Refresh));
}

void UWidgetTaskInfoBox::OnRefresh()
{
	Super::OnRefresh();

	for(auto Iter : TaskInfoItems)
	{
		Iter->Refresh();
	}
}

void UWidgetTaskInfoBox::OnTaskInfoContentRefresh()
{
	for(auto Iter : TaskInfoItems)
	{
		DestroySubWidget(Iter, true);
	}
	TaskInfoItems.Empty();

	if(UTaskBase* RootTask = UTaskModuleStatics::GetCurrentRootTask())
	{
		OnCreateTaskInfoItem(RootTask);
	}
}

void UWidgetTaskInfoBox::OnCreateTaskInfoItem(UTaskBase* InTask)
{
	if(ContentBox)
	{
		if(UWidgetTaskInfoItem* TaskInfoItem = CreateSubWidget<UWidgetTaskInfoItem>({ InTask }, TaskInfoItemClass))
		{
			TaskInfoItems.Add(TaskInfoItem);
			if(const auto VerticalBoxSlot = ContentBox->AddChildToVerticalBox(TaskInfoItem))
			{
				VerticalBoxSlot->SetHorizontalAlignment(HAlign_Left);
				VerticalBoxSlot->SetPadding(FMargin(InTask->TaskHierarchy * 30.f, 0.f, 0.f, 0.f));
			}
		}
		for(auto Iter : InTask->SubTasks)
		{
			OnCreateTaskInfoItem(Iter);
		}
	}
}
