// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/World/WidgetVitalityHP.h"

#include "Vitality/DWVitality.h"

UWidgetVitalityHP::UWidgetVitalityHP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("VitalityHP");

	WidgetAlignment = FVector2D(0.5f);
	WidgetVisibility = EWorldWidgetVisibility::ScreenAndDistance;
	WidgetShowDistance = 1800.f;
}

void UWidgetVitalityHP::OnTick_Implementation(float DeltaSeconds)
{
	Super::OnTick_Implementation(DeltaSeconds);
}

bool UWidgetVitalityHP::IsWidgetVisible_Implementation(bool bRefresh)
{
	if(!Super::IsWidgetVisible_Implementation(bRefresh)) return false;
	
	if(bRefresh)
	{
		ADWVitality* Vitality = GetOwnerObject<ADWVitality>();
		return Vitality && !Vitality->IsDead() && ISceneActorInterface::Execute_IsVisible(Vitality);
	}
	return true;
}
