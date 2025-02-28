// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAIController.h"
#include "Character/DWCharacter.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"

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
	ADWCharacter* OwnerAgent = GetPawn<ADWCharacter>();
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	if(OwnerAgent && PlayerCharacter && OwnerAgent->IsTeamMate(PlayerCharacter)) return;
	
	Super::OnTargetPerceptionUpdated(Actor, Stimulus);
}
