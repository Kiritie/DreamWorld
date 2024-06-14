// Fill out your copyright notice in the Description page of Project Settings.

#include "Vitality/States/DWVitalityState_Death.h"

#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Ability/Vitality/AbilityVitalityInventoryBase.h"
#include "Vitality/DWVitality.h"

UDWVitalityState_Death::UDWVitalityState_Death()
{
	
}

void UDWVitalityState_Death::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

void UDWVitalityState_Death::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);
}

void UDWVitalityState_Death::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWVitalityState_Death::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);
}

void UDWVitalityState_Death::OnTermination()
{
	Super::OnTermination();
}

void UDWVitalityState_Death::DeathStart()
{
	Super::DeathStart();
}

void UDWVitalityState_Death::DeathEnd()
{
	ADWVitality* Vitality = GetAgent<ADWVitality>();
	
	Vitality->Inventory->DiscardAllItem();

	Super::DeathEnd();
}
