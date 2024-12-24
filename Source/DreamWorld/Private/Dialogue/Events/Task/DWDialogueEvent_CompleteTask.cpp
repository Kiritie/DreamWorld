// Fill out your copyright notice in the Description page of Project Settings.

#include "Dialogue/Events/Task/DWDialogueEvent_CompleteTask.h"

#include "Task/TaskModuleStatics.h"
#include "Task/Base/TaskAsset.h"

UDWDialogueEvent_CompleteTask::UDWDialogueEvent_CompleteTask()
{
	TaskAsset = nullptr;
	TaskGUID = TEXT("");
	TaskResult = ETaskExecuteResult::None;
}

void UDWDialogueEvent_CompleteTask::RecieveEventTriggered_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	UTaskModuleStatics::AddTaskAsset(TaskAsset);

	UTaskModuleStatics::CompleteTaskByGUID(TaskGUID, TaskResult);
}
