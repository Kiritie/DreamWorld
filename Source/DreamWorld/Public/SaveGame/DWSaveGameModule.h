// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/SaveGameModule.h"

#include "DWSaveGameModule.generated.h"

UCLASS()
class DREAMWORLD_API UDWSaveGameModule : public USaveGameModule
{
	GENERATED_BODY()
		
	GENERATED_MODULE(UDWSaveGameModule)

public:	
	// ParamSets default values for this actor's properties
	UDWSaveGameModule();

	~UDWSaveGameModule();
	
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
