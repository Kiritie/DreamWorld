// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskPanel.h"

#include "Components/ScrollBoxSlot.h"
#include "Components/ScrollBox.h"
#include "CommonButtonBase.h"
#include "Achievement/AchievementModuleStatics.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Event/EventModuleStatics.h"
#include "Event/Handle/Task/EventHandle_TaskStateChanged.h"
#include "Task/TaskModuleStatics.h"
#include "Task/Base/TaskAsset.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Task/WidgetTaskCategory.h"
#include "Widget/Task/WidgetTaskContainer.h"
#include "Widget/Task/Info/WidgetTaskInfo.h"
#include "Widget/Task/WidgetTaskItem.h"
#include "Widget/Task/WidgetTaskRootItem.h"

UWidgetTaskPanel::UWidgetTaskPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("TaskPanel");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::UIOnly;

	WidgetType = EWidgetType::Temporary;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	bWidgetAutoFocus = true;
	
	SetIsFocusable(true);
}

void UWidgetTaskPanel::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	if(BtnStartTask)
	{
		BtnStartTask->OnClicked().AddUObject(this, &UWidgetTaskPanel::OnStartTaskButtonClicked);
	}
}

void UWidgetTaskPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetTaskPanel::OnReset(bool bForce)
{
	Super::OnReset(bForce);

	for(auto Iter : TaskCategories)
	{
		Iter->Destroy(true);
	}
	TaskCategories.Empty();

	for(auto Iter : TaskRootItems)
	{
		DestroySubWidget(Iter, true);
	}
	TaskRootItems.Empty();

	for(auto Iter : TaskInfos)
	{
		DestroySubWidget(Iter, true);
	}
	TaskInfos.Empty();

	SelectedTaskItem = nullptr;
}

void UWidgetTaskPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	UEventModuleStatics::SubscribeEvent<UEventHandle_TaskStateChanged>(this, GET_FUNCTION_NAME_THISCLASS(Refresh));

	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();

	OnTaskContentRefresh();
}

void UWidgetTaskPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_TaskStateChanged>(this, GET_FUNCTION_NAME_THISCLASS(Refresh));

	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();
}

void UWidgetTaskPanel::OnRefresh()
{
	Super::OnRefresh();

	for(auto Iter : TaskRootItems)
	{
		Iter->Refresh();
	}

	for(auto Iter : TaskInfos)
	{
		Iter->Refresh();
	}

	bool bCanStartTask = true;
	UTaskBase* SelectedTask;
	if(GetSelectedTask(SelectedTask))
	{
		FString Info;
		if(SelectedTask->TaskExecuteResult != ETaskExecuteResult::None || !SelectedTask->CheckTaskCondition(Info))
		{
			bCanStartTask = false;
		}
	}
	else
	{
		bCanStartTask = false;
	}
	
	if(BtnStartTask)
	{
		BtnStartTask->SetIsEnabled(bCanStartTask);
	}
}

void UWidgetTaskPanel::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);
}

void UWidgetTaskPanel::OnTaskRootItemSelected_Implementation(UWidgetTaskRootItem* InItem)
{
	if(InItem == SelectedTaskRootItem) return;

	if(SelectedTaskRootItem)
	{
		SelectedTaskRootItem->SetIsSelected(false, false);
	}

	SelectedTaskRootItem = InItem;
}

void UWidgetTaskPanel::OnTaskRootItemDeselected_Implementation(UWidgetTaskRootItem* InItem)
{
	if(InItem != SelectedTaskRootItem) return;

	SelectedTaskRootItem = nullptr;
}

void UWidgetTaskPanel::OnTaskItemSelected_Implementation(UWidgetTaskItem* InItem)
{
	if(InItem == SelectedTaskItem) return;

	if(SelectedTaskItem)
	{
		SelectedTaskItem->SetIsSelected(false, false);
	}

	SelectedTaskItem = InItem;

	Refresh();

	OnTaskInfoContentRefresh();
}

void UWidgetTaskPanel::OnTaskItemDeselected_Implementation(UWidgetTaskItem* InItem)
{
	if(InItem != SelectedTaskItem) return;

	SelectedTaskItem = nullptr;

	Refresh();

	for(auto Iter : TaskInfos)
	{
		DestroySubWidget(Iter, true);
	}
	TaskInfos.Empty();
}

void UWidgetTaskPanel::OnTaskContentRefresh()
{
	Reset();

	if(TaskContent)
	{
		for(auto Iter1 : UTaskModuleStatics::GetTaskAssets())
		{
			if(UWidgetTaskCategory* TaskCategory = UObjectPoolModuleStatics::SpawnObject<UWidgetTaskCategory>(this, { Iter1 }, false, TaskCategoryClass))
			{
				TaskCategories.Add(TaskCategory);
				if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(TaskContent->AddChild(TaskCategory)))
				{
					ScrollBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				}
				for(auto Iter2 : Iter1->RootTasks)
				{
					if(UWidgetTaskContainer* TaskContainer = UObjectPoolModuleStatics::SpawnObject<UWidgetTaskContainer>(this, nullptr, false, TaskContainerClass))
					{
						TaskContainer->SetVisibility(ESlateVisibility::Collapsed);
						if(UWidgetTaskRootItem* TaskRootItem = CreateSubWidget<UWidgetTaskRootItem>({ Iter2, TaskContainer, TaskCategory }, TaskRootItemClass))
						{
							TaskRootItems.Add(TaskRootItem);
							if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(TaskContent->AddChild(TaskRootItem)))
							{
								ScrollBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
							}
						}
						if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(TaskContent->AddChild(TaskContainer)))
						{
							ScrollBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
						}
					}
				}
			}
		}
	}
}

void UWidgetTaskPanel::OnTaskInfoContentRefresh()
{
	if(TaskInfoContent)
	{
		UTaskBase* SelectedTask;
		if(GetSelectedTask(SelectedTask))
		{
			for(auto Iter : TaskInfos)
			{
				DestroySubWidget(Iter, true);
			}
			TaskInfos.Empty();
			OnCreateTaskInfo(SelectedTask);
		}
	}
}

void UWidgetTaskPanel::OnStartTaskButtonClicked()
{
	if(SelectedTaskRootItem)
	{
		UTaskModuleStatics::SwitchTaskAsset(SelectedTaskRootItem->GetTaskCategory()->GetTaskAsset());
	}
	UTaskBase* SelectedTask;
	if(GetSelectedTask(SelectedTask))
	{
		if(SelectedTask->IsEntered())
		{
			UTaskModuleStatics::EnterTask(SelectedTask);
			UAchievementModuleStatics::UnlockAchievement(FName("FirstStartTaskItem"));
		}
		else
		{
			UTaskModuleStatics::LeaveTask(SelectedTask);
		}
	}
	Refresh();
}

void UWidgetTaskPanel::OnCreateTaskInfo(UTaskBase* InTask)
{
	if(TaskInfoContent)
	{
		if(UWidgetTaskInfo* TaskInfo = CreateSubWidget<UWidgetTaskInfo>({ InTask }, TaskInfoClass))
		{
			TaskInfos.Add(TaskInfo);
			if(const auto VerticalBoxSlot = TaskInfoContent->AddChildToVerticalBox(TaskInfo))
			{
				VerticalBoxSlot->SetHorizontalAlignment(HAlign_Left);
				VerticalBoxSlot->SetPadding(FMargin((InTask->TaskHierarchy - 1) * 30.f, 0.f, 0.f, 0.f));
			}
		}
		for(auto Iter : InTask->SubTasks)
		{
			OnCreateTaskInfo(Iter);
		}
	}
}

bool UWidgetTaskPanel::GetSelectedTask(UTaskBase*& OutTask) const
{
	if(SelectedTaskItem)
	{
		OutTask = SelectedTaskItem->GetTask();
		return true;
	}
	return false;
}
