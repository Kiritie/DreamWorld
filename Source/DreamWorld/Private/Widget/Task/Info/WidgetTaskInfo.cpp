// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Task/Info/WidgetTaskInfo.h"

#include "Components/TextBlock.h"
#include "Task/Base/TaskBase.h"


UWidgetTaskInfo::UWidgetTaskInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TxtName = nullptr;
	TxtProgress = nullptr;
	TxtDetail = nullptr;
	
	Task = nullptr;
}

void UWidgetTaskInfo::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
	
	Task = nullptr;
}

void UWidgetTaskInfo::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		Task = InParams[0];
	}
	Super::OnInitialize(InParams);
}

void UWidgetTaskInfo::OnRefresh()
{
	Super::OnRefresh();

	if(Task)
	{
		TxtName->SetText(Task->TaskDisplayName);
		FString ProgressStr;
		switch(Task->TaskExecuteResult)
		{
			case ETaskExecuteResult::None:
			{
				switch(Task->GetTaskState())
				{
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
					default: break;
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
		TxtDetail->SetText(!Task->TaskDescription.IsEmpty() ? Task->TaskDescription : FText::FromString(TEXT("暂无描述")));
	}
}
