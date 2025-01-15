// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Sleep.h"

#include "Common/CommonStatics.h"
#include "FSM/Components/FSMComponent.h"
#include "Voxel/VoxelModule.h"

UDWPlayerCharacterState_Sleep::UDWPlayerCharacterState_Sleep()
{
	
}

void UDWPlayerCharacterState_Sleep::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWPlayerCharacterState_Sleep::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWPlayerCharacterState_Sleep::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	UCommonStatics::SetTimeScale(10.f);
}

void UDWPlayerCharacterState_Sleep::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWPlayerCharacterState_Sleep::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	UCommonStatics::SetTimeScale(1.f);
}

void UDWPlayerCharacterState_Sleep::OnTermination()
{
	Super::OnTermination();
}
