// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Task/WidgetTaskInfoItem.h"
#include "Task/Base/TaskBase.h"


UWidgetTaskInfoItem::UWidgetTaskInfoItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetTaskInfoItem::Init_Implementation(UTaskBase* InTask)
{
	Task = InTask;
}
