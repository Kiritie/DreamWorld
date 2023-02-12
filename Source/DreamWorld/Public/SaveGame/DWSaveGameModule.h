// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/SaveGameModule.h"

#include "DWSaveGameModule.generated.h"

UCLASS()
class DREAMWORLD_API ADWSaveGameModule : public ASaveGameModule
{
	GENERATED_BODY()
		
	GENERATED_MODULE(ADWSaveGameModule)

public:	
	// ParamSets default values for this actor's properties
	ADWSaveGameModule();
	
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

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;
};
