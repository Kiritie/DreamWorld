// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ability/Vitality/States/AbilityVitalityStateBase.h"
#include "Ability/Vitality/States/AbilityVitalityState_Default.h"
#include "DWVitalityState_Default.generated.h"

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UDWVitalityState_Default : public UAbilityVitalityState_Default
{
	GENERATED_BODY()

public:
	UDWVitalityState_Default();

public:
	virtual void OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex) override;

	virtual void OnEnter(UFiniteStateBase* InLastFiniteState) override;
	
	virtual void OnRefresh() override;

	virtual void OnLeave(UFiniteStateBase* InNextFiniteState) override;

	virtual void OnTermination() override;
};
