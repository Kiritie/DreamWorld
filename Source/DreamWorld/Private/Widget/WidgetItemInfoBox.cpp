// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetItemInfoBox.h"

#include "Character/DWCharacter.h"
#include "Interaction/Components/InteractionComponent.h"
#include "Main/MainModule.h"
#include "Widget/WidgetModule.h"


UWidgetItemInfoBox::UWidgetItemInfoBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ItemInfoBox");
	WidgetType = EWidgetType::SemiTemporary;
	InputMode = EInputMode::None;
}

void UWidgetItemInfoBox::OnCreate_Implementation()
{
	Super::OnCreate_Implementation();
}

void UWidgetItemInfoBox::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetItemInfoBox::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);
}

void UWidgetItemInfoBox::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
}
