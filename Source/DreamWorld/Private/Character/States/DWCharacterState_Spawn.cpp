// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Spawn.h"

#include "Character/DWCharacter.h"
#include "Common/Looking/LookingComponent.h"
#include "Components/CapsuleComponent.h"
#include "FSM/Components/FSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"

UDWCharacterState_Spawn::UDWCharacterState_Spawn()
{
	
}

void UDWCharacterState_Spawn::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Spawn::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWCharacterState_Spawn::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->DoAction(GameplayTags::AbilityTag_Character_Action_Revive);

	Character->LimitToAnim();
	
	// stats
	Character->SetControlMode(Character->ControlMode);
	Character->GetLooking()->TargetLookingOff();
	Character->RidingTarget = nullptr;
	Character->OwnerRider = nullptr;
	
	// local
	Character->AIMoveLocation = EMPTY_Vector;
	Character->AIMoveStopDistance = 0;

	Character->GetCharacterMovement()->SetActive(false);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UDWCharacterState_Spawn::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Spawn::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->FreeToAnim();
	Character->GetCharacterMovement()->SetActive(true);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UDWCharacterState_Spawn::OnTermination()
{
	Super::OnTermination();
}

void UDWCharacterState_Spawn::TryLeave()
{
	Super::TryLeave();
}
