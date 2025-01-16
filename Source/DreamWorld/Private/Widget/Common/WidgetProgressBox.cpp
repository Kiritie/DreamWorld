// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Common/WidgetProgressBox.h"

#include "Widget/WidgetModule.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/CommonTextBlockN.h"

UWidgetProgressBox::UWidgetProgressBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ProgressBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_ProgressBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreate;
	WidgetRefreshType = EWidgetRefreshType::Tick;
	
	MaxDuration = -1.f;
	CurrentTime = 0.f;
}

void UWidgetProgressBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetProgressBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetProgressBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SetProgress(0.f);

	if(InParams.IsValidIndex(0))
	{
		TxtItemName->SetText(InParams[0]);
	}
	if(InParams.IsValidIndex(1))
	{
		TxtActionName->SetText(InParams[1]);
	}
	if(InParams.IsValidIndex(2))
	{
		MaxDuration = InParams[2];
	}
	else
	{
		MaxDuration = -1.f;
	}
	if(InParams.IsValidIndex(4))
	{
		OnCompleted = InParams[4].GetPointerValueRef<FSimpleDelegate>();
	}
	CurrentTime = 0.f;
}

void UWidgetProgressBox::OnRefresh()
{
	Super::OnRefresh();

	if(MaxDuration != -1.f)
	{
		CurrentTime += GetWorld()->GetDeltaSeconds();
		SetProgress(CurrentTime / MaxDuration);
		if(CurrentTime >= MaxDuration)
		{
			Close();
		}
	}
}

void UWidgetProgressBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	if(OnCompleted.IsBound())
	{
		OnCompleted.Execute();
		OnCompleted.Unbind();
	}
}
