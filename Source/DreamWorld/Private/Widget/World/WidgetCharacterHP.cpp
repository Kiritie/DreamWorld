// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/World/WidgetCharacterHP.h"

UWidgetCharacterHP::UWidgetCharacterHP(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("CharacterHP");
	InputMode = EInputMode::None;
	
	OwnerCharacter = nullptr;
	Visibility = ESlateVisibility::Hidden;
}
