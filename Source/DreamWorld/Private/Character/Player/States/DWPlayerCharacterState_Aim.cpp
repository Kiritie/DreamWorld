// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Aim.h"

#include "Camera/CameraModuleStatics.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "FSM/Components/FSMComponent.h"

UDWPlayerCharacterState_Aim::UDWPlayerCharacterState_Aim()
{
	StateName = FName("Aim");
}

void UDWPlayerCharacterState_Aim::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWPlayerCharacterState_Aim::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWPlayerCharacterState_Aim::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWPlayerCharacter* Character = GetAgent<ADWPlayerCharacter>();

	Character->SetUseControllerRotation(true);
	
	UCameraModuleStatics::DoCameraOffset(FVector(0.f, 30.f, 30.f), 0.5f, EEaseType::InOutSine);
	UCameraModuleStatics::DoCameraFov(60.f, 0.5f, EEaseType::InOutSine);
}

void UDWPlayerCharacterState_Aim::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWPlayerCharacterState_Aim::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWPlayerCharacter* Character = GetAgent<ADWPlayerCharacter>();

	if(!InNextState || !InNextState->IsA<UDWCharacterState_Attack>())
	{
		Character->SetUseControllerRotation(false);

		UCameraModuleStatics::DoCameraOffset(FVector(-1.f), 0.5f, EEaseType::InOutSine);
		UCameraModuleStatics::DoCameraFov(-1.f, 0.5f, EEaseType::InOutSine);
	}
}

void UDWPlayerCharacterState_Aim::OnTermination()
{
	Super::OnTermination();
}
