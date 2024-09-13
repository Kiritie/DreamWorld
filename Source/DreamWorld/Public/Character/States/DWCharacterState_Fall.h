// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Character/States/AbilityCharacterState_Fall.h"
#include "UObject/Object.h"
#include "DWCharacterState_Fall.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWCharacterState_Fall : public UAbilityCharacterState_Fall
{
	GENERATED_BODY()

	friend class ADWCharacter;

public:
	UDWCharacterState_Fall();

public:
	virtual void OnInitialize(UFSMComponent* InFSM, int32 InStateIndex) override;

	virtual bool OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;

	virtual void OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;
	
	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnLeave(UFiniteStateBase* InNextState) override;

	virtual void OnTermination() override;

protected:
	float MaxPosZ;

public:
	float GetFallHeight() const;
};
