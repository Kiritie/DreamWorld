// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Default.h"

#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "FSM/Components/FSMComponent.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"

UDWPlayerCharacterState_Default::UDWPlayerCharacterState_Default()
{
	
}

void UDWPlayerCharacterState_Default::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWPlayerCharacterState_Default::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnEnterValidate(InLastState, InParams);
}

void UDWPlayerCharacterState_Default::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();
}

void UDWPlayerCharacterState_Default::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWPlayerCharacterState_Default::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWPlayerCharacterState_Default::OnTermination()
{
	Super::OnTermination();
}

void UDWPlayerCharacterState_Default::TrySwitchToWalk()
{
	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();
	
	if(!UVoxelModule::IsExist())
	{
		FSM->SwitchStateByClass<UDWCharacterState_Walk>();
	}
	else if(UVoxelModule::Get().IsBasicGenerated())
	{
		if(PlayerCharacter->GetActorLocation().IsNearlyZero())
		{
			const auto& characterData = PlayerCharacter->GetCharacterData<UAbilityCharacterDataBase>();
			FHitResult hitResult;
			if(UVoxelModuleStatics::VoxelAgentTraceSingle(PlayerCharacter->GetActorLocation(), FVector2D(1000.f), characterData.Radius, characterData.HalfHeight, {}, hitResult, true, 100, true))
			{
				PlayerCharacter->SetActorLocationAndRotation(hitResult.Location, FRotator::ZeroRotator);
				FSM->SwitchStateByClass<UDWCharacterState_Walk>();
			}
		}
		else
		{
			FSM->SwitchStateByClass<UDWCharacterState_Walk>();
		}
	}
}
