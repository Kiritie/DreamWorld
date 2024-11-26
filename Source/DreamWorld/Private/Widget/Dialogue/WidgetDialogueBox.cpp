// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Dialogue/WidgetDialogueBox.h"

#include "Dialogue.h"
#include "Widget/WidgetModule.h"
#include "Widget/WidgetModuleStatics.h"

UWidgetDialogueBox::UWidgetDialogueBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("DialogueBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_DialogueBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreate;
	WidgetInputMode = EInputMode::UIOnly;
	
	Dialogue = nullptr;
}

void UWidgetDialogueBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetDialogueBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetDialogueBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	if(InParams.IsValidIndex(0))
	{
		Dialogue = InParams[0];
	}
	
	Super::OnOpen(InParams, bInstant);
}

void UWidgetDialogueBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}
