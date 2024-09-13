// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Default.h"

#include "Character/DWCharacter.h"
#include "Common/Looking/LookingComponent.h"
#include "Components/CapsuleComponent.h"
#include "FSM/Components/FSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"

UDWCharacterState_Default::UDWCharacterState_Default()
{
	
}

void UDWCharacterState_Default::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Default::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWCharacterState_Default::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->DoAction(EDWCharacterActionType::Revive);

	Character->LimitToAnim();
	
	// stats
	Character->SetControlMode(Character->ControlMode);
	Character->GetLooking()->TargetLookingOff();
	Character->RidingTarget = nullptr;
	Character->OwnerRider = nullptr;
	
	// local
	Character->AttackAbilityIndex = 0;
	Character->AIMoveLocation = EMPTY_Vector;
	Character->AIMoveStopDistance = 0;

	Character->GetCharacterMovement()->SetActive(false);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UDWCharacterState_Default::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Default::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->FreeToAnim();
	Character->GetCharacterMovement()->SetActive(true);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UDWCharacterState_Default::OnTermination()
{
	Super::OnTermination();
}

void UDWCharacterState_Default::TryLeave()
{
	Super::TryLeave();
}
