// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/World/WidgetVitalityHP.h"

#include "Vitality/DWVitality.h"

UWidgetVitalityHP::UWidgetVitalityHP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("VitalityHP");

	WidgetAlignment = FVector2D(0.5f);
	WidgetVisibility = EWorldWidgetVisibility::ScreenAndDistance;
	WidgetShowDistance = 1500.f;
}

void UWidgetVitalityHP::OnTick_Implementation(float DeltaSeconds)
{
	Super::OnTick_Implementation(DeltaSeconds);
}

bool UWidgetVitalityHP::IsWidgetVisible_Implementation(bool bRefresh)
{
	if(bRefresh) return Super::IsWidgetVisible_Implementation(bRefresh) && GetOwnerObject<ADWVitality>() && !GetOwnerObject<ADWVitality>()->IsDead();
	return Super::IsWidgetVisible_Implementation(bRefresh);
}
