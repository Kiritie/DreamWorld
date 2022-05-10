// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Gameplay/WHGameState.h"
#include "DWGameState.generated.h"

/**
 * ��Ϸ״̬����
 */
UCLASS()
class DREAMWORLD_API ADWGameState : public AWHGameState
{
	GENERATED_BODY()

public:
	ADWGameState();

public:
	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation() override;

protected:
	// ��ǰ״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EDWGameState CurrentState;

public:
	/** ���õ�ǰ״̬ */
	UFUNCTION(BlueprintCallable)
	void SetCurrentState(EDWGameState InGameState);

	/** ��ȡ��ǰ״̬ */
	UFUNCTION(BlueprintPure)
	EDWGameState GetCurrentState() const { return CurrentState; }
};
