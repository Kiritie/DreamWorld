// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task/TaskModule.h"

#include "DWTaskModule.generated.h"

UCLASS()
class DREAMWORLD_API UDWTaskModule : public UTaskModule
{
	GENERATED_BODY()
		
	GENERATED_MODULE(UDWTaskModule)

public:	
	// ParamSets default values for this actor's properties
	UDWTaskModule();

	~UDWTaskModule();
	
	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
#endif

	virtual void OnInitialize() override;

	virtual void OnPreparatory(EPhase InPhase) override;

	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnPause() override;

	virtual void OnUnPause() override;

	virtual void OnTermination(EPhase InPhase) override;

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;
};
