// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Common/DWCommonTypes.h"
#include "SaveGame/Base/SaveGameBase.h"

#include "DWSettingSaveGame.generated.h"

class UDWArchiveSaveGame;

/**
 * 常规游戏数据存取类
 */
UCLASS()
class DREAMWORLD_API UDWSettingSaveGame : public USaveGameBase
{
	GENERATED_BODY()

public:
	UDWSettingSaveGame();
	
public:
	virtual void OnCreate_Implementation(int32 InIndex) override;
	
	virtual void OnLoad_Implementation(EPhase InPhase) override;
	
	virtual void OnUnload_Implementation(EPhase InPhase) override;

	virtual void OnRefresh_Implementation() override;

protected:
	UPROPERTY()
	FDWSettingSaveData SaveData;
	
	UPROPERTY()
	FDWSettingSaveData DefaultData;

public:
	virtual FSaveData* GetSaveData() override { return &SaveData; }

	virtual void SetSaveData(FSaveData* InSaveData) override { SaveData = InSaveData->CastRef<FDWSettingSaveData>(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSaveData"))
	FDWSettingSaveData& K2_GetSaveData() { return SaveData; }

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetSaveData"))
	void K2_SetSaveData(const FDWSettingSaveData& InSaveData) { SaveData = InSaveData; }

	virtual FSaveData* GetDefaultData() override { return &DefaultData; }

	virtual void SetDefaultData(FSaveData* InDefaultData) override { DefaultData = InDefaultData->CastRef<FDWSettingSaveData>(); }
};
