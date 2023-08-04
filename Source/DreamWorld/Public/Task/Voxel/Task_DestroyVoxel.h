// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task/Base/Task_Base.h"
#include "Task_DestroyVoxel.generated.h"

class UEventHandle_DestroyVoxel;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UTask_DestroyVoxel : public UTask_Base
{
	GENERATED_BODY()
	
public:
	UTask_DestroyVoxel();

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
	
	virtual void OnEnter(UTaskBase* InLastTask) override;
	
	virtual void OnRefresh() override;

	virtual void OnGuide() override;

	virtual void OnExecute() override;

	virtual void OnComplete(ETaskExecuteResult InTaskExecuteResult) override;
	
	virtual void OnLeave() override;

protected:
	virtual void Serialize(FArchive& Ar) override;

	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData(bool bRefresh) override;

public:
	virtual float CheckTaskProgress_Implementation(FString& OutInfo) const override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnDestroyVoxel(UObject* InSender, UEventHandle_DestroyVoxel* InEventHandle);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentCount;
};
