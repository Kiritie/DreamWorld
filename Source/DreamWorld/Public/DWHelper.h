// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DWHelper.generated.h"

UCLASS()
class DREAMWORLD_API UDWHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//////////////////////////////////////////////////////////////////////////
	// Trace
public:
	UFUNCTION(BlueprintPure, Category = "DWHelper")
	static ETraceTypeQuery GetGameTrace(EDWGameTraceType InGameTraceType);
};
