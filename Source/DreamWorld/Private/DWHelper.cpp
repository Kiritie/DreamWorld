// Fill out your copyright notice in the Description page of Project Settings.


#include "DWHelper.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"

ETraceTypeQuery UDWHelper::GetGameTrace(EDWGameTraceType InGameTraceType)
{
	return UEngineTypes::ConvertToTraceType((ECollisionChannel)InGameTraceType);
}
