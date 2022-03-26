// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractionComponent.h"
#include "VitalityInteractionComponent.generated.h"

class IInteractionInterface;
/**
* 
*/
UCLASS()
class DREAMWORLD_API UAbilityVitalityInteractionComponent : public UInteractionComponent
{
	GENERATED_BODY()
		
public:
	UAbilityVitalityInteractionComponent(const FObjectInitializer& ObjectInitializer);

public:
	virtual bool DoInteract(IInteractionInterface* InInteractionTarget, EInteractAction InInteractAction) override;
};
