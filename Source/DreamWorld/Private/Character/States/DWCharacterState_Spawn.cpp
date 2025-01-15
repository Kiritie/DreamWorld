// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Spawn.h"

#include "Ability/Character/States/AbilityCharacterState_Static.h"
#include "Character/DWCharacter.h"
#include "FSM/Components/FSMComponent.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/DWProcedure_Testing.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Voxel/Chunks/VoxelChunk.h"

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
	
	// stats
	Character->SetControlMode(Character->ControlMode);
	
	// local
	Character->AIMoveLocation = EMPTY_Vector;
	Character->AIMoveStopDistance = 0;
}

void UDWCharacterState_Spawn::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Spawn::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWCharacterState_Spawn::OnTermination()
{
	Super::OnTermination();
}

void UDWCharacterState_Spawn::TryLeave()
{
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UDWProcedure_Testing>())
	{
		Super::TryLeave();
	}
	else
	{
		if(UVoxelModuleStatics::GetWorldMode() == EVoxelWorldMode::Default)
		{
			ADWCharacter* Character = GetAgent<ADWCharacter>();
			if(AVoxelChunk* VoxelChunk = Cast<AVoxelChunk>(ISceneActorInterface::Execute_GetContainer(Character).GetObject()))
			{
				if(VoxelChunk->IsGenerated())
				{
					Super::TryLeave();
				}
			}
		}
		else
		{
			FSM->SwitchStateByClass<UAbilityCharacterState_Static>();
		}
	}
}
