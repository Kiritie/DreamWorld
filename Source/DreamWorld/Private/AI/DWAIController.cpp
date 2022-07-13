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

	ADWCharacter* OwnerCharacter = GetPawn<ADWCharacter>();
	
	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(Actor);
	
	if (TargetCharacter && TargetCharacter != OwnerCharacter && !TargetCharacter->IsDead())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (!GetTargetCharacter())
			{
				SetTargetCharacter(TargetCharacter);
				SetLostTarget(false);
			}
			else if(TargetCharacter == GetTargetCharacter())
			{
				SetLostTarget(false);
			}
		}
		else if(TargetCharacter == GetTargetCharacter())
		{
			SetLostTarget(true, TargetCharacter->GetActorLocation());
		}
	}
}

void ADWAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADWAIController::OnTargetCharacterStateChanged(UFiniteStateBase* InFiniteState)
{
	if(InFiniteState && InFiniteState->IsA<UDWCharacterState_Death>())
	{
		SetTargetCharacter(nullptr);
		SetLostTarget(false);
	}
}

EDWCharacterNature ADWAIController::GetCharacterNature() const
{
	return (EDWCharacterNature)GetBlackboard<UDWAIBlackboard>()->GetCharacterNature();
}

void ADWAIController::SetCharacterNature(EDWCharacterNature InCharacterNature)
{
	GetBlackboard<UDWAIBlackboard>()->SetCharacterNature((uint8)InCharacterNature);
}

float ADWAIController::GetAttackDistance() const
{
	return GetBlackboard<UDWAIBlackboard>()->GetAttackDistance();
}

void ADWAIController::SetAttackDistance(float InAttackDistance)
{
	GetBlackboard<UDWAIBlackboard>()->SetAttackDistance(InAttackDistance);
}

float ADWAIController::GetFollowDistance() const
{
	return GetBlackboard<UDWAIBlackboard>()->GetFollowDistance();
}

void ADWAIController::SetFollowDistance(float InFollowDistance)
{
	GetBlackboard<UDWAIBlackboard>()->SetFollowDistance(InFollowDistance);
}

float ADWAIController::GetPatrolDistance() const
{
	return GetBlackboard<UDWAIBlackboard>()->GetPatrolDistance();
}

void ADWAIController::SetPatrolDistance(float InPatrolDistance)
{
	GetBlackboard<UDWAIBlackboard>()->SetPatrolDistance(InPatrolDistance);
}

float ADWAIController::GetPatrolDuration() const
{
	return GetBlackboard<UDWAIBlackboard>()->GetPatrolDuration();
}

void ADWAIController::SetPatrolDuration(float InPatrolDuration)
{
	GetBlackboard<UDWAIBlackboard>()->SetPatrolDuration(InPatrolDuration);
}

bool ADWAIController::IsLostTarget() const
{
	return GetBlackboard<UDWAIBlackboard>()->GetIsLostTarget();
}

void ADWAIController::SetLostTarget(bool bLostTarget, FVector InLostTargetLocation /*= FVector::ZeroVector*/)
{
	GetBlackboard<UDWAIBlackboard>()->SetIsLostTarget(bLostTarget);
	GetBlackboard<UDWAIBlackboard>()->SetLostTargetLocation(InLostTargetLocation);
}

ADWCharacter* ADWAIController::GetTargetCharacter() const
{
	return Cast<ADWCharacter>(GetBlackboard<UDWAIBlackboard>()->GetTargetCharacter());
}

void ADWAIController::SetTargetCharacter(ADWCharacter* InTargetCharacter)
{
	if(!GetBlackboardComponent() || InTargetCharacter == GetTargetCharacter()) return;

	if(GetTargetCharacter())
	{
		GetTargetCharacter()->GetFSMComponent()->OnStateChanged.RemoveDynamic(this, &ADWAIController::OnTargetCharacterStateChanged);
	}
	if(InTargetCharacter)
	{
		InTargetCharacter->GetFSMComponent()->OnStateChanged.AddDynamic(this, &ADWAIController::OnTargetCharacterStateChanged);
	}

	GetBlackboard<UDWAIBlackboard>()->SetTargetCharacter(InTargetCharacter);
}
