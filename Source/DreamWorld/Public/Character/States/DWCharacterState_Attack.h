// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ability/Character/States/AbilityCharacterStateBase.h"
#include "DWCharacterState_Attack.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWCharacterState_Attack : public UAbilityCharacterStateBase
{
	GENERATED_BODY()

	friend class ADWCharacter;

public:
	UDWCharacterState_Attack();

public:
	virtual void OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex) override;

	virtual bool OnEnterValidate(UFiniteStateBase* InLastFiniteState) override;

	virtual void OnEnter(UFiniteStateBase* InLastFiniteState) override;
	
	virtual void OnRefresh() override;

	virtual void OnLeave(UFiniteStateBase* InNextFiniteState) override;

	virtual void OnTermination() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void AttackStart();

	UFUNCTION(BlueprintCallable)
	virtual void AttackHurt();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
};
