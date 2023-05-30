// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/Task_Base.h"
#include "Task_GenerateVoxel.generated.h"

class UEventHandle_GenerateVoxel;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UTask_GenerateVoxel : public UTask_Base
{
	GENERATED_BODY()
	
public:
	UTask_GenerateVoxel();

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

public:
	virtual float CheckTaskProgress_Implementation(FString& OutInfo) const override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnGenerateVoxel(UObject* InSender, UEventHandle_GenerateVoxel* InEventHandle);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentCount;
};
