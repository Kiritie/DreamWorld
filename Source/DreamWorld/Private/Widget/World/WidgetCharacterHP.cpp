// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/World/WidgetCharacterHP.h"

#include "Character/DWCharacter.h"

UWidgetCharacterHP::UWidgetCharacterHP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("CharacterHP");
	
	WidgetAlignment = FVector2D(0.5f);
	WidgetVisibility = EWorldWidgetVisibility::ScreenAndDistance;
	WidgetShowDistance = 1800.f;

	bWidgetTickAble = true;
}

void UWidgetCharacterHP::OnTick_Implementation(float DeltaSeconds)
{
	Super::OnTick_Implementation(DeltaSeconds);
}

bool UWidgetCharacterHP::IsWidgetVisible_Implementation(bool bRefresh)
{
	if(!Super::IsWidgetVisible_Implementation(bRefresh)) return false;
	
	if(bRefresh)
	{
		ADWCharacter* Character = GetOwnerObject<ADWCharacter>();
		return Character && !Character->IsPlayer() && !Character->IsDead() && ISceneActorInterface::Execute_IsVisible(Character);
	}
	return true;
}
