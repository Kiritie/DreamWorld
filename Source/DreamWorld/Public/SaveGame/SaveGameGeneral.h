// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SaveGame/SaveGameBase.h"

#include "SaveGameGeneral.generated.h"

class USaveGameArchive;

/**
 * 常规游戏数据存取类
 */
UCLASS()
class DREAMWORLD_API USaveGameGeneral : public USaveGameBase
{
	GENERATED_BODY()

public:
	USaveGameGeneral();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGeneralSaveData GeneralData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, FArchiveSaveData> ArchiveDatas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString CurrentArchiveName;

public:
	virtual void RefreshSaveData_Implementation() override;

	FGeneralSaveData& GetGeneralData() { return GeneralData; }

	void SetGeneralData(FGeneralSaveData InGeneralData) { GeneralData = InGeneralData; }

	TMap<FString, FArchiveSaveData>& GetArchiveDatas() { return ArchiveDatas; }

	FString GetCurrentArchiveName() const { return CurrentArchiveName; }

	void SetCurrentArchiveName(const FString& InCurrentArchiveName) { this->CurrentArchiveName = InCurrentArchiveName; }
};
