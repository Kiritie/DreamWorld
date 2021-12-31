// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetPrimaryPanel.h"

#include "Character/DWCharacter.h"
#include "Interaction/Components/InteractionComponent.h"
#include "Main/MainModule.h"
#include "Widget/WidgetModule.h"


UWidgetPrimaryPanel::UWidgetPrimaryPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("PrimaryPanel");
	WidgetType = EWidgetType::Permanent;
	InputMode = EInputMode::None;
}

void UWidgetPrimaryPanel::OnCreate_Implementation()
{
	Super::OnCreate_Implementation();
}
