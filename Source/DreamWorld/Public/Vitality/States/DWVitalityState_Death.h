// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Vitality/States/AbilityVitalityState_Death.h"
#include "UObject/Object.h"
#include "DWVitalityState_Death.generated.h"

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UDWVitalityState_Death : public UAbilityVitalityState_Death
{
	GENERATED_BODY()

public:
	UDWVitalityState_Death();

public:
	virtual void OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex) override;

	virtual void OnEnter(UFiniteStateBase* InLastFiniteState) override;
	
	virtual void OnRefresh() override;

	virtual void OnLeave(UFiniteStateBase* InNextFiniteState) override;

	virtual void OnTermination() override;

public:
	virtual void DeathStart() override;

	virtual void DeathEnd() override;
};
