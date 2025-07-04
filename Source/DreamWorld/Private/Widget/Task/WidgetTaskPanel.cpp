// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskPanel.h"

#include "Components/ScrollBoxSlot.h"
#include "Components/ScrollBox.h"
#include "CommonButtonBase.h"
#include "Asset/AssetModuleStatics.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Event/EventModuleStatics.h"
#include "Event/Handle/Task/EventHandle_TaskStateChanged.h"
#include "Task/TaskModuleStatics.h"
#include "Task/Base/TaskAsset.h"
#include "Task/Base/DWTaskBase.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Context/WidgetContextBox.h"
#include "Widget/Item/WidgetAbilityItem.h"
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

	if(BtnTraceTask)
	{
		BtnTraceTask->OnClicked().AddUObject(this, &UWidgetTaskPanel::OnTraceTaskButtonClicked);
	}
}

void UWidgetTaskPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetTaskPanel::OnReset(bool bForce)
{
	Super::OnReset(bForce);

	OnTaskItemDeselected(SelectedTaskItem);
	OnTaskRootItemDeselected(SelectedTaskRootItem);

	for(auto Iter : TaskCategories)
	{
		DestroySubWidget(Iter, true);
	}
	TaskCategories.Empty();
}

void UWidgetTaskPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	OnTaskContentRefresh(true);

	Super::OnOpen(InParams, bInstant);

	UEventModuleStatics::SubscribeEvent<UEventHandle_TaskStateChanged>(this, GET_FUNCTION_NAME_THISCLASS(Refresh));

	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();
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

	for(auto Iter : TaskCategories)
	{
		Iter->Refresh();
	}

	for(auto Iter : TaskInfos)
	{
		Iter->Refresh();
	}

	for(auto Iter : PreviewItems)
	{
		Iter->Refresh();
	}

	bool bCanTraceTask = true;
	UTaskBase* SelectedTask;
	if(GetSelectedTask(SelectedTask))
	{
		if(SelectedTask->IsLeaved())
		{
			bCanTraceTask = false;
		}
		if(UDWTaskBase* Task = Cast<UDWTaskBase>(SelectedTask))
		{
			SelectedPreviewItems = Task->Prizes;
		}
	}
	else
	{
		bCanTraceTask = false;
	}
	
	if(BtnTraceTask)
	{
		BtnTraceTask->SetIsEnabledN(bCanTraceTask);
	}
}

void UWidgetTaskPanel::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);
}

void UWidgetTaskPanel::OnTaskRootItemSelected_Implementation(UWidgetTaskRootItem* InItem)
{
	if(InItem == SelectedTaskRootItem) return;

	if(SelectedTaskItem)
	{
		SelectedTaskItem->SetIsSelected(false, false);
	}

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

	OnTaskItemDeselected(SelectedTaskItem);
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

	OnPreviewContentRefresh();
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

	for(auto Iter : PreviewItems)
	{
		DestroySubWidget(Iter, true);
	}
	PreviewItems.Empty();
}

void UWidgetTaskPanel::OnTaskContentRefresh(bool bScrollToStart)
{
	Reset();

	if(TaskContent)
	{
		for(auto Iter1 : UTaskModuleStatics::GetTaskAssets())
		{
			if(UWidgetTaskCategory* TaskCategory = GetOrCreateTaskCategory(Iter1))
			{
				for(auto Iter2 : Iter1->RootTasks)
				{
					if(Iter2->IsLeaved(true)) continue;
					
					if(UWidgetTaskRootItem* TaskRootItem = CreateSubWidget<UWidgetTaskRootItem>({ Iter2, Iter1 }, TaskRootItemClass))
					{
						TaskCategory->GetTaskContainer()->AddTaskItem(TaskRootItem);
					}
				}
			}
		}
		
		if(bScrollToStart)
		{
			TaskContent->ScrollToStart();
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

void UWidgetTaskPanel::OnPreviewContentRefresh()
{
	if(PreviewContent)
	{
		UTaskBase* SelectedTask;
		if(GetSelectedTask(SelectedTask))
		{
			for(auto Iter : PreviewItems)
			{
				DestroySubWidget(Iter, true);
			}
			PreviewItems.Empty();
			for(auto& Iter : SelectedPreviewItems)
			{
				if(UWidgetAbilityItem* PreviewItem = CreateSubWidget<UWidgetAbilityItem>({ &Iter }, UAssetModuleStatics::GetStaticClass(FName("SimplePreviewItem"))))
				{
					PreviewItems.Add(PreviewItem);
					if(UWrapBoxSlot* WrapBoxSlot = PreviewContent->AddChildToWrapBox(PreviewItem))
					{
						WrapBoxSlot->SetPadding(FMargin(5.f, 5.f, 5.f, 5.f));
					}
				}
			}
		}
	}
}

void UWidgetTaskPanel::OnTraceTaskButtonClicked()
{
	UTaskBase* SelectedTask;
	if(GetSelectedTask(SelectedTask))
	{
		if(!SelectedTask->IsCompleted(true))
		{
			if(!SelectedTask->IsCurrent())
			{
				if(UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>())
				{
					UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("追踪任务: %s"), *SelectedTask->TaskDisplayName.ToString()));
				}
				UTaskModuleStatics::SetCurrentTask(SelectedTask);
			}
			else
			{
				if(UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>())
				{
					UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("停止追踪任务: %s"), *SelectedTask->TaskDisplayName.ToString()));
				}
				UTaskModuleStatics::SetCurrentTask(nullptr);
			}
		}
		else
		{
			UTaskModuleStatics::LeaveTask(SelectedTask);
			OnTaskContentRefresh();
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

UWidgetTaskCategory* UWidgetTaskPanel::GetOrCreateTaskCategory(UTaskAsset* InTaskAsset)
{
	if(InTaskAsset->IsAllTaskLeaved()) return nullptr;
	
	for(auto Iter : TaskCategories)
	{
		if(Iter->GetTaskName().EqualTo(InTaskAsset->DisplayName))
		{
			return Iter;
		}
	}
	if(UWidgetTaskCategory* TaskCategory = CreateSubWidget<UWidgetTaskCategory>({ InTaskAsset->DisplayName }, TaskCategoryClass))
	{
		TaskCategories.Add(TaskCategory);
		if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(TaskContent->AddChild(TaskCategory)))
		{
			ScrollBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
		}
		return TaskCategory;
	}
	return nullptr;
}
