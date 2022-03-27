// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/World/WidgetVitalityHP.h"

UWidgetVitalityHP::UWidgetVitalityHP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("VitalityHP");
	InputMode = EInputMode::None;
	
	OwnerObject = nullptr;
	Visibility = ESlateVisibility::Hidden;
}
