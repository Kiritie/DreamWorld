// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Default.h"

#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Character/DWCharacter.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "Components/CapsuleComponent.h"
#include "FSM/Components/FSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Voxel/Chunks/VoxelChunk.h"

UDWCharacterState_Default::UDWCharacterState_Default()
{
	
}

void UDWCharacterState_Default::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Default::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWCharacterState_Default::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->DoAction(EDWCharacterActionType::Revive);

	Character->LimitToAnim();
	
	// stats
	Character->SetControlMode(Character->ControlMode);
	Character->SetLockedTarget(nullptr);
	Character->RidingTarget = nullptr;
	Character->OwnerRider = nullptr;
	
	// local
	Character->AttackAbilityIndex = 0;
	Character->AIMoveLocation = Vector_Empty;
	Character->AIMoveStopDistance = 0;

	Character->GetCharacterMovement()->SetActive(false);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UDWCharacterState_Default::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Default::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->FreeToAnim();
	Character->GetCharacterMovement()->SetActive(true);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UDWCharacterState_Default::OnTermination()
{
	Super::OnTermination();
}

void UDWCharacterState_Default::TrySwitchToWalk()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if(!Character->IsPlayer())
	{
		FSM->SwitchStateByClass<UDWCharacterState_Walk>();
	}
	else if(Character->GetActorLocation().IsNearlyZero())
	{
		const auto& characterData = Character->GetCharacterData<UAbilityCharacterDataBase>();
		FHitResult hitResult;
		if(UVoxelModuleBPLibrary::VoxelAgentTraceSingle(Character->GetActorLocation(), FVector2D(1000.f), characterData.Radius, characterData.HalfHeight, {}, hitResult, true, 10, true))
		{
			Character->SetActorLocationAndRotation(hitResult.Location, FRotator::ZeroRotator);
			FSM->SwitchStateByClass<UDWCharacterState_Walk>();
		}
	}
}
