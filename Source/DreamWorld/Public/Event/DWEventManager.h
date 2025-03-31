// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Event/Manager/DefaultEventManagerBase.h"
#include "DWEventManager.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWEventManager : public UDefaultEventManagerBase
{
	GENERATED_BODY()

public:
	virtual void OnInitialize() override;
};
