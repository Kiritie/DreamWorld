// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SaveGame/SaveGameBase.h"
#include "SaveGame/SaveGameDataBase.h"

#include "ArchiveSaveGame.generated.h"

/**
 * 世界数据存取类
 */
UCLASS()
class DREAMWORLD_API UArchiveSaveGame : public USaveGameBase
{
	GENERATED_BODY()

public:
	UArchiveSaveGame();

public:
	virtual void OnCreate_Implementation(USaveGameDataBase* InSaveGameData) override;

	virtual void OnLoad_Implementation() override;

	virtual void OnUnload_Implementation() override;
	
	virtual void OnRefresh_Implementation() override;
};

UCLASS()
class DREAMWORLD_API UArchiveSaveGameData : public USaveGameDataBase
{
	GENERATED_BODY()

public:
	UArchiveSaveGameData();

public:
	virtual void OnInitialize_Implementation() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (InstanceEditable, ExposeOnSpawn))
	FArchiveSaveData ArchiveData;
};
