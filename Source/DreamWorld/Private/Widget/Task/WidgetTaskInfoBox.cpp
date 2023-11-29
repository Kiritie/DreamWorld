// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Task/WidgetTaskInfoBox.h"

#include "Asset/AssetModuleStatics.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Event/EventModuleStatics.h"
#include "Event/Handle/Task/EventHandle_EnterTask.h"
#include "Task/TaskModuleStatics.h"
#include "Task/Base/TaskBase.h"
#include "Widget/WidgetModule.h"
#include "Widget/Task/WidgetTaskInfoItem.h"


UWidgetTaskInfoBox::UWidgetTaskInfoBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("TaskInfoBox");
	ParentName = FName("GameHUD");
	WidgetType = EWidgetType::Permanent;
	WidgetInputMode = EInputMode::None;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
}

void UWidgetTaskInfoBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetTaskInfoBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetTaskInfoBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	UEventModuleStatics::SubscribeEvent<UEventHandle_EnterTask>(this, FName("Refresh"));
}

void UWidgetTaskInfoBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_EnterTask>(this, FName("Refresh"));
}

void UWidgetTaskInfoBox::OnRefresh()
{
	Super::OnRefresh();

	if(UTaskBase* RootTask = UTaskModuleStatics::GetCurrentRootTask())
	{
		ContentBox->ClearChildren();
		CreateTaskInfoItem(RootTask);
	}
}

void UWidgetTaskInfoBox::CreateTaskInfoItem(UTaskBase* InTask)
{
	if(UWidgetTaskInfoItem* TaskInfoItem = CreateSubWidget<UWidgetTaskInfoItem>({ InTask }, UAssetModuleStatics::GetStaticClass(FName("TaskInfoItem"))))
	{
		if(const auto TempSlot = ContentBox->AddChildToVerticalBox(TaskInfoItem))
		{
			TempSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
			TempSlot->SetPadding(FMargin(InTask->TaskHierarchy * 50.f, 0.f, 0.f, 0.f));
		}
	}
	for(auto Iter : InTask->SubTasks)
	{
		CreateTaskInfoItem(Iter);
	}
}
