// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ability/Character/States/AbilityCharacterStateBase.h"
#include "Ability/Character/States/AbilityCharacterState_Jump.h"
#include "DWCharacterState_Jump.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWCharacterState_Jump : public UAbilityCharacterState_Jump
{
	GENERATED_BODY()

	friend class ADWCharacter;

public:
	UDWCharacterState_Jump();

public:
	virtual void OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex) override;

	virtual bool OnEnterValidate(UFiniteStateBase* InLastFiniteState) override;

	virtual void OnEnter(UFiniteStateBase* InLastFiniteState) override;
	
	virtual void OnRefresh() override;

	virtual void OnLeave(UFiniteStateBase* InNextFiniteState) override;

	virtual void OnTermination() override;
};
