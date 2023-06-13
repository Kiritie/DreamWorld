// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Base/AIControllerBase.h"
#include "Perception/AIPerceptionTypes.h"
#include "DWAIController.generated.h"

DECLARE_DELEGATE_TwoParams(FDWAIPerceptionUpdated, class ADWCharacter*, bool)

/**
 * AI����������
 */
UCLASS()
class DREAMWORLD_API ADWAIController : public AAIControllerBase
{
    GENERATED_BODY()

public:
    ADWAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

public:
	virtual void Tick(float DeltaTime) override;
};
