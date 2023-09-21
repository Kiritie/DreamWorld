// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Task/WidgetTaskInfoItem.h"

#include "Task/Base/TaskBase.h"


UWidgetTaskInfoItem::UWidgetTaskInfoItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetTaskInfoItem::OnInitialize_Implementation(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		Task = InParams[0].GetObjectValue<UTaskBase>();
	}
	Super::OnInitialize_Implementation(InParams);
}

void UWidgetTaskInfoItem::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}
