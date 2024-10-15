// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Character/States/DWCharacterState_Spawn.h"
#include "DWPlayerCharacterState_Spawn.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWPlayerCharacterState_Spawn : public UDWCharacterState_Spawn
{
	GENERATED_BODY()

	friend class ADWPlayerCharacter;

public:
	UDWPlayerCharacterState_Spawn();

public:
	virtual void OnInitialize(UFSMComponent* InFSM, int32 InStateIndex) override;

	virtual bool OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;

	virtual void OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;
	
	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnLeave(UFiniteStateBase* InNextState) override;

	virtual void OnTermination() override;

protected:
	virtual void TryLeave() override;
};
