// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Video/VideoModule.h"

#include "DWVideoModule.generated.h"

UCLASS()
class DREAMWORLD_API ADWVideoModule : public AVideoModule
{
	GENERATED_BODY()
	
	GENERATED_MODULE(ADWVideoModule)

public:	
	// ParamSets default values for this actor's properties
	ADWVideoModule();
	
	~ADWVideoModule();

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

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual void UnloadData(EPhase InPhase) override;

	virtual FSaveData* ToData() override;
};
