// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Common/DWCommonTypes.h"
#include "SaveGame/Module/ModuleSaveGame.h"

#include "DWSettingSaveGame.generated.h"

class UDWArchiveSaveGame;

/**
 * 常规游戏数据存取类
 */
UCLASS()
class DREAMWORLD_API UDWSettingSaveGame : public UModuleSaveGame
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
	FDWSettingModuleSaveData SaveData;
	
	UPROPERTY()
	FDWSettingModuleSaveData DefaultData;

public:
	virtual FSaveData* GetSaveData() override { return &SaveData; }

	virtual void SetSaveData(FSaveData* InSaveData) override { SaveData = InSaveData->CastRef<FDWSettingModuleSaveData>(); }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSaveData"))
	FDWSettingModuleSaveData& K2_GetSaveData() { return SaveData; }

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetSaveData"))
	void K2_SetSaveData(const FDWSettingModuleSaveData& InSaveData) { SaveData = InSaveData; }

	virtual FSaveData* GetDefaultData() override { return &DefaultData; }

	virtual void SetDefaultData(FSaveData* InDefaultData) override { DefaultData = InDefaultData->CastRef<FDWSettingModuleSaveData>(); }
};
