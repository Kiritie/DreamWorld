// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityModule.h"

#include "DWAbilityModule.generated.h"

UCLASS()
class DREAMWORLD_API UDWAbilityModule : public UAbilityModule
{
	GENERATED_BODY()
		
	GENERATED_MODULE(UDWAbilityModule)

public:	
	// ParamSets default values for this actor's properties
	UDWAbilityModule();
	
	~UDWAbilityModule();
	
	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
#endif

	virtual void OnInitialize() override;

	virtual void OnPreparatory(EPhase InPhase) override;

	virtual void OnRefresh(float DeltaSeconds, bool bInEditor) override;

	virtual void OnPause() override;

	virtual void OnUnPause() override;

public:
	virtual ECollisionChannel GetPickUpTraceChannel() const override;
};
