// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Achievement/AchievementModule.h"

#include "DWAchievementModule.generated.h"

UCLASS()
class DREAMWORLD_API ADWAchievementModule : public AAchievementModule
{
	GENERATED_BODY()
	
	GENERATED_MODULE(ADWAchievementModule)

public:	
	// ParamSets default values for this actor's properties
	ADWAchievementModule();
	
	~ADWAchievementModule();

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
