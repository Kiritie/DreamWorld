// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractionComponent.h"
#include "VitalityInteractionComponent.generated.h"

class IInteraction;
/**
* 
*/
UCLASS()
class DREAMWORLD_API UVitalityInteractionComponent : public UInteractionComponent
{
	GENERATED_BODY()
		
	public:
	UVitalityInteractionComponent(const FObjectInitializer& ObjectInitializer);

	public:
	virtual bool DoInteract(IInteraction* InInteractionTarget, EInteractAction InInteractAction) override;
};
