// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DreamWorld.h"
#include "GameFramework/GameStateBase.h"
#include "DWGameState.generated.h"

/**
 * ��Ϸ״̬����
 */
UCLASS()
class ADWGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ADWGameState();

protected:
	// ��ǰ״̬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameState CurrentState;

protected:
	virtual void BeginPlay() override;

public:
	/** ���õ�ǰ״̬ */
	UFUNCTION(BlueprintCallable)
	void SetCurrentState(EGameState InGameState);

	/** ��ȡ��ǰ״̬ */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EGameState GetCurrentState() const { return CurrentState; }
};
