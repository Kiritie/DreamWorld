// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskItem.h"

#include "Components/TextBlock.h"
#include "Task/Base/TaskBase.h"
#include "Task/Base/Task_Base.h"
#include "Widget/Task/WidgetTaskPanel.h"

UWidgetTaskItem::UWidgetTaskItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = true;
	bToggleable = true;

	TxtName = nullptr;
	TxtLevel = nullptr;
	TxtProgress = nullptr;
	TxtDetail = nullptr;

	Task = nullptr;
}

void UWidgetTaskItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	Task = nullptr;
}

void UWidgetTaskItem::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		Task = InParams[0];
	}
	Super::OnInitialize(InParams);
}

void UWidgetTaskItem::OnRefresh()
{
	Super::OnRefresh();

	if(Task)
	{
		TxtName->SetText(Task->TaskDisplayName);
		if(UTask_Base* TaskBase = Cast<UTask_Base>(Task))
		{
			if(TaskBase->Level > 0)
			{
				TxtLevel->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), TaskBase->Level)));
			}
			else
			{
				TxtLevel->SetText(FText::FromString(TEXT("无限制")));
			}
		}
		FString ProgressStr;
		switch(Task->TaskExecuteResult)
		{
			case ETaskExecuteResult::None:
			{
				switch(Task->GetTaskState())
				{
					case ETaskState::Entered:
					case ETaskState::Executing:
					{
						ProgressStr = TEXT("进行中");
						break;
					}
					default:
					{
						ProgressStr = TEXT("未开始");
						break;
					}
				}
				break;
			}
			default:
			{
				ProgressStr = TEXT("已完成");
				break;
			}
		}
		TxtProgress->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *ProgressStr)));
		TxtDetail->SetText(Task->TaskDescription);
	}
}

void UWidgetTaskItem::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);

	if(UWidgetTaskPanel* TaskPanel = GetOwnerWidget<UWidgetTaskPanel>())
	{
		TaskPanel->OnTaskItemSelected(this);
	}
}

void UWidgetTaskItem::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);

	if(UWidgetTaskPanel* TaskPanel = GetOwnerWidget<UWidgetTaskPanel>())
	{
		TaskPanel->OnTaskItemDeselected(this);
	}
}
