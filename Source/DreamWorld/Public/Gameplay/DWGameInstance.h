// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Gameplay/WHGameInstance.h"
#include "DWGameInstance.generated.h"

/**
 * 游戏实例基类
 */
UCLASS()
class DREAMWORLD_API UDWGameInstance : public UWHGameInstance
{
	GENERATED_BODY()

public:
	UDWGameInstance();

public:
	virtual void Init() override;

	virtual void Shutdown() override;
};
