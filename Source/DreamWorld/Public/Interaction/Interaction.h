// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interaction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DREAMWORLD_API IInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanInteract(IInteraction* InInteractionTarget, EInteractAction InInteractAction) = 0;

	virtual void OnInteract(IInteraction* InInteractionTarget, EInteractAction InInteractAction) = 0;

public:
	virtual class UInteractionComponent* GetInteractionComponent() const = 0;
};
