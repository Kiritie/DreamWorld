// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Default.h"

#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "FSM/Components/FSMComponent.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/Procedure_Testing.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Voxel/Chunks/VoxelChunk.h"

UDWPlayerCharacterState_Default::UDWPlayerCharacterState_Default()
{
	
}

void UDWPlayerCharacterState_Default::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWPlayerCharacterState_Default::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWPlayerCharacterState_Default::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
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

void UDWPlayerCharacterState_Default::TryLeave()
{
	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Testing>())
	{
		Super::TryLeave();
	}
	else if(UVoxelModule::Get().IsBasicGenerated())
	{
		if(PlayerCharacter->GetActorLocation().IsNearlyZero())
		{
			const auto& CharacterData = PlayerCharacter->GetCharacterData<UAbilityCharacterDataBase>();
			FHitResult HitResult;
			if(UVoxelModuleStatics::VoxelAgentTraceSingle(PlayerCharacter->GetActorLocation(), FVector2D(1000.f), CharacterData.Radius, CharacterData.HalfHeight, {}, HitResult, true, 100, true))
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
