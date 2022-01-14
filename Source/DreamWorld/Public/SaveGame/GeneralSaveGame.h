// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SaveGame/SaveGameBase.h"

#include "GeneralSaveGame.generated.h"

class UArchiveSaveGame;

/**
 * 常规游戏数据存取类
 */
UCLASS()
class DREAMWORLD_API UGeneralSaveGame : public USaveGameBase
{
	GENERATED_BODY()

public:
	UGeneralSaveGame();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGeneralSaveData GeneralData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FArchiveBasicSaveData> ArchiveBasicDatas;

public:
	virtual void OnCreate_Implementation(USaveGameDataBase* InSaveGameData) override;
	
	virtual void OnLoad_Implementation() override;
	
	virtual void OnRefresh_Implementation() override;
};
