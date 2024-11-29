// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Audio/AudioModuleTypes.h"
#include "Procedure/Base/ProcedureBase.h"
#include "DWProcedureBase.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWProcedureBase : public UProcedureBase
{
	GENERATED_BODY()
	
public:
	UDWProcedureBase();

	//////////////////////////////////////////////////////////////////////////
	/// Procedure
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnUnGenerate() override;
#endif

public:
	virtual void OnInitialize() override;
	
	virtual void OnEnter(UProcedureBase* InLastProcedure) override;
	
	virtual void OnRefresh() override;

	virtual void OnGuide() override;
	
	virtual void OnLeave(UProcedureBase* InNextProcedure) override;

protected:
	UPROPERTY(EditAnywhere, Category = "BGM")
	USoundBase* BGMSound;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotationSpeed;

	static FSingleSoundHandle BGMHandle;
};
