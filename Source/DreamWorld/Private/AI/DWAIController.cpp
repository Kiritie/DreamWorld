// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DWAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Character/DWCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/DWAIBlackboard.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Character/States/DWCharacterState_Death.h"
#include "FSM/Components/FSMComponent.h"

ADWAIController::ADWAIController()
{
	
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

void ADWAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADWCharacter* OwnerCharacter = GetPawn<ADWCharacter>();

	if(!OwnerCharacter) return;

	if(IsRunningBehaviorTree())
	{
		if(OwnerCharacter->IsDead())
		{
			StopBehaviorTree();
		}
	}
}
