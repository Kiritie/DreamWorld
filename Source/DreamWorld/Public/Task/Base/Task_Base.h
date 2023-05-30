// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task/Base/TaskBase.h"
#include "Task_Base.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UTask_Base : public UTaskBase
{
	GENERATED_BODY()
	
public:
	UTask_Base();

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
	virtual bool CheckTaskCondition_Implementation(FString& OutInfo) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NeedLevel;
};
