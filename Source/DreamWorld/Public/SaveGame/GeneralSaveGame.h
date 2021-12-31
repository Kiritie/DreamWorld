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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGeneralSaveData GeneralData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, FArchiveSaveData> ArchiveDatas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName CurrentArchiveID;

public:
	virtual void RefreshSaveData_Implementation() override;

	FGeneralSaveData& GetGeneralData() { return GeneralData; }

	void SetGeneralData(FGeneralSaveData InGeneralData) { GeneralData = InGeneralData; }

	TMap<FName, FArchiveSaveData>& GetArchiveDatas() { return ArchiveDatas; }

	FName GetCurrentArchiveID() const { return CurrentArchiveID; }

	void SetCurrentArchiveID(FName InCurrentArchiveID) { this->CurrentArchiveID = InCurrentArchiveID; }
};
