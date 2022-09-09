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

	Character->SetMana(Character->GetMaxMana());
	Character->SetStamina(Character->GetMaxStamina());
	
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

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if(Character->IsPlayer())
	{
		if(Character->GetActorLocation().IsNearlyZero())
		{
			if(UVoxelModuleBPLibrary::IsBasicGenerated())
			{
				const auto& characterData = Character->GetCharacterData();
				const auto& worldData = AVoxelModule::Get()->GetWorldData();
				const float& chunkRadius = worldData.GetChunkLength() * 0.5f;
				DON(i, 10,
					const FVector rayStart = FVector(i == 0 ? 0.f : worldData.RandomStream.FRandRange(-chunkRadius, chunkRadius), i == 0 ? 0.f : worldData.RandomStream.FRandRange(-chunkRadius, chunkRadius), worldData.GetWorldHeight(true));
					const FVector rayEnd = FVector(rayStart.X, rayStart.Y, 0.f);
					FHitResult hitResult1;
					if(UVoxelModuleBPLibrary::ChunkTraceSingle(rayStart, rayEnd, characterData.Radius, characterData.HalfHeight, {}, hitResult1))
					{
						FHitResult hitResult2;
						if(!UVoxelModuleBPLibrary::VoxelTraceSingle(hitResult1.Location, hitResult1.Location, characterData.Radius * 0.95f, characterData.HalfHeight * 0.95f, {}, hitResult2))
						{
							Character->SetActorLocationAndRotation(hitResult1.Location, FRotator::ZeroRotator);
							FSM->SwitchStateByClass<UDWCharacterState_Walk>();
							break;
						}
					}
				)
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

	Character->FreeToAnim();
	Character->GetCharacterMovement()->SetActive(true);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UDWCharacterState_Default::OnTermination()
{
	Super::OnTermination();
}
