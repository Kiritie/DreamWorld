// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModule.h"

#include "DWCameraModule.generated.h"

UCLASS()
class DREAMWORLD_API ADWCameraModule : public ACameraModule
{
	GENERATED_BODY()
	
	GENERATED_MODULE(ADWCameraModule)

public:	
	// ParamSets default values for this actor's properties
	ADWCameraModule();
	
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

public:
	virtual void SetCameraDistance(float InDistance, bool bInstant) override;
};
