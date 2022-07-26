// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Default.h"

#include "Character/DWCharacter.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "Components/CapsuleComponent.h"
#include "FSM/Components/FSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleBPLibrary.h"

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

	Character->SetMana(-1.f);
	Character->SetStamina(-1.f);
	
	// stats
	Character->SetControlMode(Character->ControlMode);
	Character->SetLockedTarget(nullptr);
	Character->RidingTarget = nullptr;
	Character->OwnerRider = nullptr;
	
	// local
	Character->AttackAbilityIndex = 0;
	Character->AIMoveLocation = Vector_Empty;
	Character->AIMoveStopDistance = 0;
	Character->NormalAttackRemainTime = 0;

	Character->GetCharacterMovement()->SetActive(false);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UDWCharacterState_Default::OnRefresh()
{
	Super::OnRefresh();

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if(Character->IsPlayer())
	{
		if(Character->GetActorLocation().IsNearlyZero())
		{
			switch(UVoxelModuleBPLibrary::GetWorldState())
			{
				case EVoxelWorldState::BasicGenerated:
				case EVoxelWorldState::FullGenerated:
				{
					FHitResult hitResult;
					const FVector rayStart = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, UVoxelModuleBPLibrary::GetWorldData().ChunkHeightRange * UVoxelModuleBPLibrary::GetWorldData().GetChunkLength() + 500);
					const FVector rayEnd = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, 0);
					if(AMainModule::GetModuleByClass<AVoxelModule>()->ChunkTraceSingle(rayStart, rayEnd, Character->GetRadius(), Character->GetHalfHeight(), hitResult))
					{
						Character->SetActorLocationAndRotation(hitResult.Location, FRotator::ZeroRotator);
						FSM->SwitchStateByClass<UDWCharacterState_Walk>();
					}
					break;
				}
				default: break;
			}
		}
		else
		{
			FSM->SwitchStateByClass<UDWCharacterState_Walk>();
		}
	}
	else
	{
		FSM->SwitchStateByClass<UDWCharacterState_Walk>();
	}
}

void UDWCharacterState_Default::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetCharacterMovement()->SetActive(true);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UDWCharacterState_Default::OnTermination()
{
	Super::OnTermination();
}
