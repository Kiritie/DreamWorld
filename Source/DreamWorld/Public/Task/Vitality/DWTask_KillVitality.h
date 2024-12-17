// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task/Base/DWTaskBase.h"
#include "DWTask_KillVitality.generated.h"

class UEventHandle_VitalityDead;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWTask_KillVitality : public UDWTaskBase
{
	GENERATED_BODY()
	
public:
	UDWTask_KillVitality();

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

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnVitalityDead(UObject* InSender, UEventHandle_VitalityDead* InEventHandle);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId TargetVitalityID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TargetCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentCount;
};
