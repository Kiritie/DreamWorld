// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SaveGame/SaveGameBase.h"

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FArchiveSaveData SaveData;

public:
	virtual void OnCreate_Implementation() override;

	virtual void OnLoad_Implementation() override;

	virtual void OnUnload_Implementation() override;
	
	virtual void OnRefresh_Implementation() override;
};
