// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Gameplay/WHGameState.h"
#include "DWGameState.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API ADWGameState : public AWHGameState
{
	GENERATED_BODY()

public:
	ADWGameState();

	//////////////////////////////////////////////////////////////////////////
	/// WHActor
public:
	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation(EPhase InPhase) override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnTermination_Implementation() override;
};
