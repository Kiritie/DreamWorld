// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskItem.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Task/Base/TaskBase.h"
#include "Widget/Task/WidgetTaskContainer.h"
#include "Widget/Task/WidgetTaskPanel.h"

UWidgetTaskItem::UWidgetTaskItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = true;
	bToggleable = true;

	TxtName = nullptr;
	TxtProgress = nullptr;
	TxtDetail = nullptr;

	Task = nullptr;
}

void UWidgetTaskItem::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
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
		if(TxtProgress)
		{
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
							ProgressStr = TEXT("进行中");
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
		}
		TxtDetail->SetText(!Task->TaskDescription.IsEmpty() ? Task->TaskDescription : FText::FromString(TEXT("暂无描述")));
	}
}

void UWidgetTaskItem::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);

	Task = nullptr;
}

void UWidgetTaskItem::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);

	if(bBroadcast)
	{
		if(UWidgetTaskPanel* TaskPanel = GetOwnerWidget<UWidgetTaskPanel>())
		{
			TaskPanel->OnTaskItemSelected(this);
		}
	}
}

void UWidgetTaskItem::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);

	if(bBroadcast)
	{
		if(UWidgetTaskPanel* TaskPanel = GetOwnerWidget<UWidgetTaskPanel>())
		{
			TaskPanel->OnTaskItemDeselected(this);
		}
	}
}

void UWidgetTaskItem::OnAddToContainer(UWidgetTaskContainer* InTaskContainer)
{
	if(UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(InTaskContainer->GetTaskContent()->AddChild(this)))
	{
		VerticalBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
	}
}
