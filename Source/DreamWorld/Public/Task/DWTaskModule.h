// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task/TaskModule.h"

#include "DWTaskModule.generated.h"

UCLASS()
class DREAMWORLD_API ADWTaskModule : public ATaskModule
{
	GENERATED_BODY()
		
	GENERATED_MODULE(ADWTaskModule)

public:	
	// ParamSets default values for this actor's properties
	ADWTaskModule();

	~ADWTaskModule();
	
	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
#endif

	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation(EPhase InPhase) override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;

	virtual void OnTermination_Implementation(EPhase InPhase) override;

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;
};
