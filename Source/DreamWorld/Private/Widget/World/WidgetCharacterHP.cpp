// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/World/WidgetCharacterHP.h"

#include "Character/DWCharacter.h"
#include "Vitality/DWVitality.h"

UWidgetCharacterHP::UWidgetCharacterHP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("CharacterHP");
	
	WidgetAlignment = FVector2D(0.5f);
	bWidgetAutoVisibility = true;
	WidgetShowDistance = 1000.f;
}

void UWidgetCharacterHP::OnTick_Implementation(float DeltaSeconds)
{
	Super::OnTick_Implementation(DeltaSeconds);
}

void UWidgetCharacterHP::RefreshVisibility_Implementation()
{
	Super::RefreshVisibility_Implementation();

	if(GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		if(GetOwnerObject<ADWCharacter>() && GetOwnerObject<ADWCharacter>()->IsDead())
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
