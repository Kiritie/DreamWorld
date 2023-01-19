// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityModule.h"

#include "DWAbilityModule.generated.h"

UCLASS()
class DREAMWORLD_API ADWAbilityModule : public AAbilityModule
{
	GENERATED_BODY()
		
	MODULE_INSTANCE_DECLARE(ADWAbilityModule)

public:	
	// ParamSets default values for this actor's properties
	ADWAbilityModule();
	
	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate_Implementation() override;

	virtual void OnDestroy_Implementation() override;
#endif

	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation(EPhase InPhase) override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;
};
