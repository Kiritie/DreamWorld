// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Ride.h"

UDWCharacterState_Ride::UDWCharacterState_Ride()
{
	StateName = FName("Ride");
}

void UDWCharacterState_Ride::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWCharacterState_Ride::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWCharacterState_Ride::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Ride::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
}

void UDWCharacterState_Ride::OnTermination()
{
	Super::OnTermination();
}
