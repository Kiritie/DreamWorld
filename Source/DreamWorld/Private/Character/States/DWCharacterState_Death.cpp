// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Death.h"

#include "Character/DWCharacter.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"

UDWCharacterState_Death::UDWCharacterState_Death()
{
	
}

void UDWCharacterState_Death::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Death::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWCharacterState_Death::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->SetMana(0.f);
	Character->SetStamina(0.f);

	Character->LeaveTeam();
}

void UDWCharacterState_Death::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Death::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWCharacterState_Death::OnTermination()
{
	Super::OnTermination();
}

void UDWCharacterState_Death::DeathStart()
{
	Super::DeathStart();
}

void UDWCharacterState_Death::DeathEnd()
{
	Super::DeathEnd();
}
