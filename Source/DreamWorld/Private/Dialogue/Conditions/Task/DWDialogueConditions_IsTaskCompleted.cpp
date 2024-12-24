// Fill out your copyright notice in the Description page of Project Settings.

#include "Dialogue/Conditions/Task/DWDialogueConditions_IsTaskCompleted.h"

#include "Task/TaskModuleStatics.h"
#include "Task/Base/TaskBase.h"

UDWDialogueConditions_IsTaskCompleted::UDWDialogueConditions_IsTaskCompleted()
{
	TaskGUID = TEXT("");
}

bool UDWDialogueConditions_IsTaskCompleted::IsConditionMet_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	if(UTaskBase* TaskBase = UTaskModuleStatics::GetTaskByGUID(TaskGUID))
	{
		return TaskBase->IsCompleted();
	}
	return false;
}
