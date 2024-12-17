// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Task/Info/WidgetTaskInfoBox.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Event/EventModuleStatics.h"
#include "Event/Handle/Task/EventHandle_CurrentTaskChanged.h"
#include "Event/Handle/Task/EventHandle_TaskStateChanged.h"
#include "Task/TaskModuleStatics.h"
#include "Task/Base/TaskBase.h"
#include "Widget/WidgetModule.h"
#include "Widget/Task/Info/WidgetTaskInfo.h"


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

	UEventModuleStatics::SubscribeEvent<UEventHandle_CurrentTaskChanged>(this, GET_FUNCTION_NAME_THISCLASS(OnTaskInfoContentRefresh));
	UEventModuleStatics::SubscribeEvent<UEventHandle_TaskStateChanged>(this, GET_FUNCTION_NAME_THISCLASS(Refresh));
	
	OnTaskInfoContentRefresh();
}

void UWidgetTaskInfoBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_CurrentTaskChanged>(this, GET_FUNCTION_NAME_THISCLASS(OnTaskInfoContentRefresh));
	UEventModuleStatics::UnsubscribeEvent<UEventHandle_TaskStateChanged>(this, GET_FUNCTION_NAME_THISCLASS(Refresh));
}

void UWidgetTaskInfoBox::OnRefresh()
{
	Super::OnRefresh();

	for(auto Iter : TaskInfos)
	{
		Iter->Refresh();
	}
}

void UWidgetTaskInfoBox::OnTaskInfoContentRefresh()
{
	for(auto Iter : TaskInfos)
	{
		DestroySubWidget(Iter, true);
	}
	TaskInfos.Empty();

	if(UTaskBase* Task = UTaskModuleStatics::GetCurrentTask())
	{
		OnCreateTaskInfo(Task);
	}
}

void UWidgetTaskInfoBox::OnCreateTaskInfo(UTaskBase* InTask)
{
	if(ContentBox)
	{
		if(UWidgetTaskInfo* TaskInfo = CreateSubWidget<UWidgetTaskInfo>({ InTask }, TaskInfoClass))
		{
			TaskInfos.Add(TaskInfo);
			if(const auto VerticalBoxSlot = ContentBox->AddChildToVerticalBox(TaskInfo))
			{
				VerticalBoxSlot->SetHorizontalAlignment(HAlign_Left);
				VerticalBoxSlot->SetPadding(FMargin(InTask->TaskHierarchy * 30.f, 0.f, 0.f, 0.f));
			}
		}
		for(auto Iter : InTask->SubTasks)
		{
			if(Iter->IsLeaved()) continue;
			
			OnCreateTaskInfo(Iter);
		}
	}
}
