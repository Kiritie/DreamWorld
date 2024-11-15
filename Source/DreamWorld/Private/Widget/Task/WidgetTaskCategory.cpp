// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Task/WidgetTaskCategory.h"

#include "Components/TextBlock.h"
#include "Task/Base/TaskAsset.h"
#include "Widget/Task/WidgetTaskPanel.h"

UWidgetTaskCategory::UWidgetTaskCategory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSelectable = false;

	TxtName = nullptr;

	TaskAsset = nullptr;
}

void UWidgetTaskCategory::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	TaskAsset = nullptr;
}

void UWidgetTaskCategory::OnInitialize(const TArray<FParameter>& InParams)
{
	if(InParams.IsValidIndex(0))
	{
		TaskAsset = InParams[0];
	}
	Super::OnInitialize(InParams);
}

void UWidgetTaskCategory::OnRefresh()
{
	Super::OnRefresh();

	if(TaskAsset)
	{
		TxtName->SetText(TaskAsset->DisplayName);
	}
}
