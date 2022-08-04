// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityModuleTypes.h"
#include "UObject/Interface.h"
#include "InventoryAgentInterface.generated.h"

class UInventory;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryAgentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DREAMWORLD_API IInventoryAgentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnSelectedItemChange(const FAbilityItem& InItem) = 0;

public:
	virtual UInventory* GetInventory() const = 0;
};
