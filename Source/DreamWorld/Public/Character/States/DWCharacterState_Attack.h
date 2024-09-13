// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
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
	virtual void OnInitialize(UFSMComponent* InFSM, int32 InStateIndex) override;

	virtual bool OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;

	virtual void OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;
	
	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnLeave(UFiniteStateBase* InNextState) override;

	virtual void OnTermination() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void AttackStart();

	UFUNCTION(BlueprintCallable)
	virtual void AttackStep();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

protected:
	FSimpleDelegate OnAttackStart;
	
	FSimpleDelegate OnAttackEnd;
};
