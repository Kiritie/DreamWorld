// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Character/States/AbilityCharacterState_Idle.h"
#include "UObject/Object.h"
#include "DWCharacterState_Idle.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWCharacterState_Idle : public UAbilityCharacterState_Idle
{
	GENERATED_BODY()

	friend class ADWCharacter;

public:
	UDWCharacterState_Idle();

public:
	virtual void OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex) override;

	virtual bool OnValidate() override;

	virtual void OnEnter(UFiniteStateBase* InLastFiniteState) override;
	
	virtual void OnRefresh() override;

	virtual void OnLeave(UFiniteStateBase* InNextFiniteState) override;

	virtual void OnTermination() override;
};
