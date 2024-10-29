// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Human/DWHumanCharacter.h"
#include "DWNPCCharacter.generated.h"

/**
 * 类人角色
 */
UCLASS()
class DREAMWORLD_API ADWNPCCharacter : public ADWHumanCharacter
{
	GENERATED_BODY()

public:
	ADWNPCCharacter(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent) override;
	
	virtual void OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive) override;
};
