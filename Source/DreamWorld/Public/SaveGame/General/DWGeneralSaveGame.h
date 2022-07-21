// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SaveGame/General/GeneralSaveGame.h"

#include "DWGeneralSaveGame.generated.h"

class UDWArchiveSaveGame;

/**
 * 常规游戏数据存取类
 */
UCLASS()
class DREAMWORLD_API UDWGeneralSaveGame : public UGeneralSaveGame
{
	GENERATED_BODY()

public:
	UDWGeneralSaveGame();
	
public:
	virtual void OnCreate_Implementation(int32 InSaveIndex) override;
	
	virtual void OnLoad_Implementation(bool bForceMode) override;
	
	virtual void OnUnload_Implementation(bool bForceMode) override;

	virtual void OnRefresh_Implementation() override;

protected:
	UPROPERTY()
	FDWGeneralSaveData DWGeneralSaveData;

public:
	virtual FSaveData* GetSaveData() override { return &DWGeneralSaveData; }

	virtual void SetSaveData(FSaveData* InSaveData) override { DWGeneralSaveData = InSaveData->CastRef<FDWGeneralSaveData>(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSaveData"))
	FDWGeneralSaveData& K2_GetSaveData() { return DWGeneralSaveData; }

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetSaveData"))
	void K2_SetSaveData(const FDWGeneralSaveData& InSaveData) { this->DWGeneralSaveData = InSaveData; }

	UFUNCTION(BlueprintPure)
	bool IsAutoJump() const { return DWGeneralSaveData.bAutoJump; }
	
	UFUNCTION(BlueprintCallable)
	void SetAutoJump(bool bInAutoJump) { DWGeneralSaveData.bAutoJump = bInAutoJump; }

	UFUNCTION(BlueprintPure)
	float GetCameraDistance() const { return DWGeneralSaveData.CameraDistance; }
	
	UFUNCTION(BlueprintCallable)
	void SetCameraDistance(float InCameraDistance) { DWGeneralSaveData.CameraDistance = InCameraDistance; }
};
