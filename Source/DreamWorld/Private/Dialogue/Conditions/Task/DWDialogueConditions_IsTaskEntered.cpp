// Fill out your copyright notice in the Description page of Project Settings.

#include "Dialogue/Conditions/Task/DWDialogueConditions_IsTaskEntered.h"

#include "Task/TaskModuleStatics.h"
#include "Task/Base/TaskBase.h"

UDWDialogueConditions_IsTaskEntered::UDWDialogueConditions_IsTaskEntered()
{
	TaskGUID = TEXT("");
}

bool UDWDialogueConditions_IsTaskEntered::IsConditionMet_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	if(UTaskBase* TaskBase = UTaskModuleStatics::GetTaskByGUID(TaskGUID))
	{
		return TaskBase->IsEntered();
	}
	return false;
}
