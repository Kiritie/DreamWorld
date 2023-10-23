// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Task/WidgetTaskInfoBox.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Event/EventModuleBPLibrary.h"
#include "Event/Handle/Task/EventHandle_EnterTask.h"
#include "Task/TaskModuleBPLibrary.h"
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

void UWidgetTaskInfoBox::OnCreate(UObject* InOwner)
{
	Super::OnCreate(InOwner);
}

void UWidgetTaskInfoBox::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetTaskInfoBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	UEventModuleBPLibrary::SubscribeEvent<UEventHandle_EnterTask>(this, FName("Refresh"));
}

void UWidgetTaskInfoBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UEventModuleBPLibrary::UnsubscribeEvent<UEventHandle_EnterTask>(this, FName("Refresh"));
}

void UWidgetTaskInfoBox::OnRefresh()
{
	Super::OnRefresh();

	if(UTaskBase* RootTask = UTaskModuleBPLibrary::GetCurrentRootTask())
	{
		ContentBox->ClearChildren();
		CreateTaskInfoItem(RootTask);
	}
}

void UWidgetTaskInfoBox::CreateTaskInfoItem(UTaskBase* InTask)
{
	if(UWidgetTaskInfoItem* TaskInfoItem = CreateSubWidget<UWidgetTaskInfoItem>({ InTask }, UAssetModuleBPLibrary::GetStaticClass(FName("TaskInfoItem"))))
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
