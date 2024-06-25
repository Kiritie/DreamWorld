// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAIController.h"
#include "Character/DWCharacter.h"

ADWAIController::ADWAIController()
{
	
}

void ADWAIController::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ADWAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ADWAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	Super::OnTargetPerceptionUpdated(Actor, Stimulus);
}
