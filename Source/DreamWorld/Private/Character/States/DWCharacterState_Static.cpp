// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Static.h"

#include "AbilitySystemComponent.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Components/CapsuleComponent.h"
#include "FSM/Components/FSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleBPLibrary.h"

UDWCharacterState_Static::UDWCharacterState_Static()
{
	StateName = FName("Static");
}

void UDWCharacterState_Static::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Static::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWCharacterState_Static::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->LimitToAnim();
}

void UDWCharacterState_Static::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Static::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Static::OnTermination()
{
	Super::OnTermination();

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->FreeToAnim();
}
