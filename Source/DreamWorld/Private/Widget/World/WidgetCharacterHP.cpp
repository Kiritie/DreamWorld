// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/World/WidgetCharacterHP.h"

#include "Character/DWCharacter.h"

UWidgetCharacterHP::UWidgetCharacterHP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("CharacterHP");
	
	WidgetAlignment = FVector2D(0.5f);
	WidgetVisibility = EWorldWidgetVisibility::ScreenAndDistance;
	WidgetShowDistance = 1500.f;
}

void UWidgetCharacterHP::OnTick_Implementation(float DeltaSeconds)
{
	Super::OnTick_Implementation(DeltaSeconds);
}

bool UWidgetCharacterHP::IsWidgetVisible_Implementation(bool bRefresh)
{
	if(bRefresh) return Super::IsWidgetVisible_Implementation(bRefresh) && GetOwnerObject<ADWCharacter>() && ISceneActorInterface::Execute_IsVisible(GetOwnerObject<ADWCharacter>()) && !GetOwnerObject<ADWCharacter>()->IsDead();
	return Super::IsWidgetVisible_Implementation(bRefresh);
}
