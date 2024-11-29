// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityModuleTypes.h"
#include "Task/Base/TaskBase.h"
#include "DWTaskBase.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWTaskBase : public UTaskBase
{
	GENERATED_BODY()
	
public:
	UDWTaskBase();

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAbilityItem> Prizes;
};
