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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGeneralSaveData SaveData;
	
public:
	virtual void OnCreate_Implementation() override;
	
	virtual void OnLoad_Implementation() override;
	
	virtual void OnUnload_Implementation() override;

	virtual void OnRefresh_Implementation() override;
};
