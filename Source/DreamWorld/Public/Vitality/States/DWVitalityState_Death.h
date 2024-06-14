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
class DREAMWORLD_API UDWVitalityState_Death : public UAbilityVitalityState_Death
{
	GENERATED_BODY()

	friend class ADWVitality;

public:
	UDWVitalityState_Death();

public:
	virtual void OnInitialize(UFSMComponent* InFSM, int32 InStateIndex) override;

	virtual void OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;
	
	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnLeave(UFiniteStateBase* InNextState) override;

	virtual void OnTermination() override;

public:
	virtual void DeathStart() override;

	virtual void DeathEnd() override;
};
