// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Gameplay/WHGameMode.h"
#include "DWGameMode.generated.h"

/**
 * 游戏模式基类
 */
UCLASS()
class DREAMWORLD_API ADWGameMode : public AWHGameMode
{
	GENERATED_BODY()

public:
	ADWGameMode();

public:
	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation() override;
};
