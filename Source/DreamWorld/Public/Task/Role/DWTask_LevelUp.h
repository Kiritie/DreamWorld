// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task/Base/DWTaskBase.h"
#include "DWTask_LevelUp.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWTask_LevelUp : public UDWTaskBase
{
	GENERATED_BODY()
	
public:
	UDWTask_LevelUp();

	//////////////////////////////////////////////////////////////////////////
	/// Task
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnUnGenerate() override;
#endif

public:
	virtual void OnInitialize() override;
	
	virtual void OnRestore() override;
	
	virtual void OnEnter() override;
	
	virtual void OnRefresh() override;

	virtual void OnGuide() override;

	virtual void OnExecute() override;

	virtual void OnComplete(ETaskExecuteResult InTaskExecuteResult) override;
	
	virtual void OnLeave() override;

protected:
	virtual void Serialize(FArchive& Ar) override;

	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

public:
	virtual float CheckTaskProgress_Implementation(FString& OutInfo) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TargetLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentLevel;
};
