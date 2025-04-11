// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Spawn.h"

#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "FSM/Components/FSMComponent.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/DWProcedure_Testing.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Voxel/Chunks/VoxelChunk.h"

UDWPlayerCharacterState_Spawn::UDWPlayerCharacterState_Spawn()
{
	
}

void UDWPlayerCharacterState_Spawn::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWPlayerCharacterState_Spawn::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWPlayerCharacterState_Spawn::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWPlayerCharacterState_Spawn::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWPlayerCharacterState_Spawn::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWPlayerCharacterState_Spawn::OnTermination()
{
	Super::OnTermination();
}

void UDWPlayerCharacterState_Spawn::TryLeave()
{
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UDWProcedure_Testing>())
	{
		Super::TryLeave();
	}
	else if(UVoxelModule::Get().IsWorldBasicGenerated())
	{
		ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();
		if(PlayerCharacter->GetActorLocation().Z <= 0.f)
		{
			const auto& CharacterData = PlayerCharacter->GetCharacterData<UAbilityCharacterDataBase>();
			FHitResult HitResult;
			if(UVoxelModuleStatics::VoxelAgentTraceSingle(PlayerCharacter->GetActorLocation(), FVector2D(1000.f), CharacterData.Radius, CharacterData.HalfHeight, {}, HitResult, true, 100, true, true))
			{
				PlayerCharacter->SetActorLocationAndRotation(HitResult.Location, FRotator::ZeroRotator);
				Super::TryLeave();
			}
		}
		else if(AVoxelChunk* VoxelChunk = Cast<AVoxelChunk>(ISceneActorInterface::Execute_GetContainer(PlayerCharacter).GetObject()))
		{
			if(VoxelChunk->IsGenerated())
			{
				Super::TryLeave();
			}
		}
	}
}
