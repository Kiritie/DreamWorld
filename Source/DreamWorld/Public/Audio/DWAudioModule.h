// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Audio/AudioModule.h"

#include "DWAudioModule.generated.h"

UCLASS()
class DREAMWORLD_API UDWAudioModule : public UAudioModule
{
	GENERATED_BODY()
	
	GENERATED_MODULE(UDWAudioModule)

public:	
	// ParamSets default values for this actor's properties
	UDWAudioModule();
	
	~UDWAudioModule();

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

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual void UnloadData(EPhase InPhase) override;

	virtual FSaveData* ToData() override;
};
