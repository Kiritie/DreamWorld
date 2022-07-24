// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/States/DWCharacterState_Death.h"
#include "UObject/Object.h"
#include "DWPlayerCharacterState_Death.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWPlayerCharacterState_Death : public UDWCharacterState_Death
{
	GENERATED_BODY()

	friend class ADWPlayerCharacter;

public:
	UDWPlayerCharacterState_Death();

public:
	virtual void OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex) override;

	virtual bool OnEnterValidate(UFiniteStateBase* InLastFiniteState) override;

	virtual void OnEnter(UFiniteStateBase* InLastFiniteState) override;
	
	virtual void OnRefresh() override;

	virtual void OnLeave(UFiniteStateBase* InNextFiniteState) override;

	virtual void OnTermination() override;

public:
	virtual void DeathStart() override;

	virtual void DeathEnd() override;
};
