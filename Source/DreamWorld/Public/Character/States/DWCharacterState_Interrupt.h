// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ability/Character/States/AbilityCharacterStateBase.h"
#include "DWCharacterState_Interrupt.generated.h"

/**
 * 
 */
UCLASS()
class WHFRAMEWORK_API UDWCharacterState_Interrupt : public UAbilityCharacterStateBase
{
	GENERATED_BODY()

public:
	UDWCharacterState_Interrupt();

public:
	virtual void OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex) override;

	virtual bool OnValidate() override;

	virtual void OnEnter(UFiniteStateBase* InLastFiniteState) override;
	
	virtual void OnRefresh() override;

	virtual void OnLeave(UFiniteStateBase* InNextFiniteState) override;

	virtual void OnTermination() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Duration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RemainTime;
};
