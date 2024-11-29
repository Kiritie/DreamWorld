// Fill out your copyright notice in the Description page of Project Settings.

#include "Dialogue/Events/DWDialogueEvent_TakeTask.h"

#include "Task/TaskModuleStatics.h"
#include "Task/Base/TaskAsset.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Context/WidgetContextBox.h"

UDWDialogueEvent_TakeTask::UDWDialogueEvent_TakeTask()
{
	TaskAsset = nullptr;
}

void UDWDialogueEvent_TakeTask::RecieveEventTriggered_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	UTaskModuleStatics::AddTaskAsset(TaskAsset);

	if(UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("已接受任务: %s"), *TaskAsset->DisplayName.ToString()));
	}
}
