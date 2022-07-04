// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ability/Character/States/AbilityCharacterStateBase.h"
#include "DWCharacterState_Swim.generated.h"

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UDWCharacterState_Swim : public UAbilityCharacterStateBase
{
	GENERATED_BODY()

public:
	UDWCharacterState_Swim();

public:
	virtual void OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex) override;

	virtual void OnEnter(UFiniteStateBase* InLastFiniteState) override;
	
	virtual void OnRefresh() override;

	virtual void OnLeave(UFiniteStateBase* InNextFiniteState) override;

	virtual void OnTermination() override;
};
