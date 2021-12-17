// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractionComponent.h"
#include "CharacterInteractionComponent.generated.h"

class IInteraction;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UCharacterInteractionComponent : public UInteractionComponent
{
	GENERATED_BODY()
		
public:
	UCharacterInteractionComponent(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool DoInteract(IInteraction* InInteractionTarget, EInteractAction InInteractAction) override;
};
