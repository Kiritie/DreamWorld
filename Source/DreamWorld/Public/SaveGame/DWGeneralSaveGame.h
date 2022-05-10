// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SaveGame/Base/SaveGameBase.h"

#include "DWGeneralSaveGame.generated.h"

class UDWArchiveSaveGame;

/**
 * 常规游戏数据存取类
 */
UCLASS()
class DREAMWORLD_API UDWGeneralSaveGame : public USaveGameBase
{
	GENERATED_BODY()

public:
	UDWGeneralSaveGame();
	
public:
	virtual void OnCreate_Implementation(int32 InSaveIndex) override;
	
	virtual void OnLoad_Implementation() override;
	
	virtual void OnUnload_Implementation() override;

	virtual void OnRefresh_Implementation() override;

public:
	UFUNCTION(BlueprintPure)
	int32 GetValidArchiveID();

protected:
	UPROPERTY()
	FDWGeneralSaveData SaveData;

public:
	UFUNCTION(BlueprintPure)
	bool IsAutoJump() const { return SaveData.bAutoJump; }
	
	UFUNCTION(BlueprintCallable)
	void SetAutoJump(bool bInAutoJump) { SaveData.bAutoJump = bInAutoJump; }

	UFUNCTION(BlueprintPure)
	float GetCameraDistance() const { return SaveData.CameraDistance; }
	
	UFUNCTION(BlueprintCallable)
	void SetCameraDistance(float InCameraDistance) { SaveData.CameraDistance = InCameraDistance; }

	UFUNCTION(BlueprintPure)
	int32 GetCurrentArchiveID() const { return SaveData.CurrentArchiveID; }
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentArchiveID(int32 InArchiveID) { SaveData.CurrentArchiveID = InArchiveID; }

	UFUNCTION(BlueprintPure)
	TMap<int32, FDWArchiveBasicSaveData>& GetArchiveBasicDatas() { return SaveData.ArchiveBasicDatas; }

	UFUNCTION(BlueprintCallable)
	void SetArchiveBasicDatas(const TMap<int32, FDWArchiveBasicSaveData>& ArchiveBasicDatas) { SaveData.ArchiveBasicDatas = ArchiveBasicDatas; }
};
