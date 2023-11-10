// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Video/VideoModule.h"

#include "DWVideoModule.generated.h"

UCLASS()
class DREAMWORLD_API UDWVideoModule : public UVideoModule
{
	GENERATED_BODY()
	
	GENERATED_MODULE(UDWVideoModule)

public:	
	// ParamSets default values for this actor's properties
	UDWVideoModule();
	
	~UDWVideoModule();

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

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual void UnloadData(EPhase InPhase) override;

	virtual FSaveData* ToData() override;
};
