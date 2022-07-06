// Fill out your copyright notice in the Description page of Project Settings.

#include "Vitality/States/DWVitalityState_Death.h"

#include "AbilitySystemComponent.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Vitality/DWVitality.h"

UDWVitalityState_Death::UDWVitalityState_Death()
{
	
}

void UDWVitalityState_Death::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

void UDWVitalityState_Death::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);
}

void UDWVitalityState_Death::OnRefresh()
{
	Super::OnRefresh();
}

void UDWVitalityState_Death::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);
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
	Super::DeathEnd();

	ADWVitality* Vitality = GetAgent<ADWVitality>();
	
	Vitality->Inventory->DiscardAllItem();
	Vitality->OwnerChunk->DestroySceneActor(Vitality);
}
