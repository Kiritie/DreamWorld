// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/World/WidgetVitalityHP.h"

#include "Vitality/DWVitality.h"

UWidgetVitalityHP::UWidgetVitalityHP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("VitalityHP");

	WidgetAlignment = FVector2D(0.5f);
	bWidgetAutoVisibility = true;
	WidgetShowDistance = 1000.f;
}

void UWidgetVitalityHP::OnTick_Implementation(float DeltaSeconds)
{
	Super::OnTick_Implementation(DeltaSeconds);
}

void UWidgetVitalityHP::RefreshVisibility_Implementation()
{
	Super::RefreshVisibility_Implementation();

	if(GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		if(GetOwnerActor<ADWVitality>() && GetOwnerActor<ADWVitality>()->IsDead())
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
