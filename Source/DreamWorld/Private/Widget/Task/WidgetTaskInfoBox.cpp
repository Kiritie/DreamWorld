// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Task/WidgetTaskInfoBox.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/DWCharacter.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Event/EventModuleBPLibrary.h"
#include "Event/Handle/Task/EventHandle_EnterTask.h"
#include "Main/MainModule.h"
#include "Task/TaskModuleBPLibrary.h"
#include "Task/Base/TaskBase.h"
#include "Widget/WidgetModule.h"
#include "Widget/Task/WidgetTaskInfoItem.h"


UWidgetTaskInfoBox::UWidgetTaskInfoBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("TaskInfoBox");
	ParentName = FName("GameHUD");
	WidgetType = EWidgetType::Permanent;
	InputMode = EInputMode::None;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;

	TaskInfoItemClass = LoadClass<UWidgetTaskInfoItem>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/Task/WB_TaskInfoItem.WB_TaskInfoItem_C'"));
}

void UWidgetTaskInfoBox::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);
}

void UWidgetTaskInfoBox::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetTaskInfoBox::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	UEventModuleBPLibrary::SubscribeEvent<UEventHandle_EnterTask>(this, FName("Refresh"));
}

void UWidgetTaskInfoBox::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	UEventModuleBPLibrary::UnsubscribeEvent<UEventHandle_EnterTask>(this, FName("Refresh"));
}

void UWidgetTaskInfoBox::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

	if(UTaskBase* RootTask = UTaskModuleBPLibrary::GetCurrentRootTask())
	{
		ContentBox->ClearChildren();
		CreateTaskInfoItem(RootTask);
	}
}

void UWidgetTaskInfoBox::CreateTaskInfoItem(UTaskBase* InTask)
{
	if(UWidgetTaskInfoItem* TaskInfoItem = CreateSubWidget<UWidgetTaskInfoItem>({ InTask }, TaskInfoItemClass))
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
