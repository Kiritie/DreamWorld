// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Task/WidgetTaskInfoItem.h"

#include "Components/TextBlock.h"
#include "Task/Base/TaskBase.h"


UWidgetTaskInfoItem::UWidgetTaskInfoItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TxtName = nullptr;
	TxtProgress = nullptr;
	TxtDetail = nullptr;
	
	Task = nullptr;
}

void UWidgetTaskInfoItem::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
	
	Task = nullptr;
}

void UWidgetTaskInfoItem::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		Task = InParams[0];
	}
	Super::OnInitialize(InParams);
}

void UWidgetTaskInfoItem::OnRefresh()
{
	Super::OnRefresh();

	if(Task)
	{
		TxtName->SetText(Task->TaskDisplayName);
		FString ProgressStr;
		switch(Task->GetTaskState())
		{
			case ETaskState::None:
			case ETaskState::Entered:
			{
				ProgressStr = TEXT("未开始");
				break;
			}
			case ETaskState::Executing:
			{
				FString TaskProgressInfo;
				Task->CheckTaskProgress(TaskProgressInfo);
				ProgressStr = TaskProgressInfo.IsEmpty() ? TEXT("进行中") : TaskProgressInfo;
				break;
			}
			case ETaskState::Completed:
			case ETaskState::Leaved:
			{
				ProgressStr = TEXT("已完成");
				break;
			}
		}
		TxtProgress->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *ProgressStr)));
		TxtDetail->SetText(Task->TaskDescription);
	}
}
