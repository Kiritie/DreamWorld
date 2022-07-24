// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Character/States/DWCharacterState_Default.h"
#include "DWPlayerCharacterState_Default.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWPlayerCharacterState_Default : public UDWCharacterState_Default
{
	GENERATED_BODY()

	friend class ADWPlayerCharacter;

public:
	UDWPlayerCharacterState_Default();

public:
	virtual void OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex) override;

	virtual bool OnEnterValidate(UFiniteStateBase* InLastFiniteState) override;

	virtual void OnEnter(UFiniteStateBase* InLastFiniteState) override;
	
	virtual void OnRefresh() override;

	virtual void OnLeave(UFiniteStateBase* InNextFiniteState) override;

	virtual void OnTermination() override;
};
