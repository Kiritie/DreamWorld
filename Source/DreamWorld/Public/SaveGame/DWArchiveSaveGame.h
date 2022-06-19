// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SaveGame/Base/SaveGameBase.h"

#include "DWArchiveSaveGame.generated.h"

/**
 * 世界数据存取类
 */
UCLASS()
class DREAMWORLD_API UDWArchiveSaveGame : public USaveGameBase
{
	GENERATED_BODY()

public:
	UDWArchiveSaveGame();

public:
	virtual void OnCreate_Implementation(int32 InSaveIndex) override;

	virtual void OnSave_Implementation() override;

	virtual void OnLoad_Implementation() override;

	virtual void OnUnload_Implementation() override;
	
	virtual void OnRefresh_Implementation() override;

	virtual void OnDestroy_Implementation() override;
	
protected:
	UPROPERTY()
	FDWArchiveSaveData SaveData;
	
public:
	UFUNCTION(BlueprintPure)
	FDWArchiveSaveData& GetSaveData() { return SaveData; }

	UFUNCTION(BlueprintCallable)
	void SetSaveData(const FDWArchiveSaveData& InSaveData) { this->SaveData = InSaveData; }

	UFUNCTION(BlueprintPure)
	bool IsPreview() const { return SaveIndex == -1; }
};
