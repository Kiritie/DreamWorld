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
class DREAMWORLD_API UDWVitalityState_Default : public UAbilityVitalityState_Default
{
	GENERATED_BODY()

	friend class ADWVitality;

public:
	UDWVitalityState_Default();

public:
	virtual void OnInitialize(UFSMComponent* InFSM, int32 InStateIndex) override;

	virtual void OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;
	
	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnLeave(UFiniteStateBase* InNextState) override;

	virtual void OnTermination() override;
};
