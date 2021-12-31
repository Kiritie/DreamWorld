// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SaveGame/SaveGameBase.h"

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FArchiveSaveData ArchiveData;

public:
	virtual void RefreshSaveData_Implementation() override;

	FArchiveSaveData& GetArchiveData() { return ArchiveData; }

	void SetArchiveData(const FArchiveSaveData& InArchiveSaveData) { this->ArchiveData = InArchiveSaveData; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FVector, FChunkSaveData> ChunkDatas;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsExistChunkData(FIndex InChunkIndex);

	UFUNCTION(BlueprintCallable)
	void SaveChunkData(FIndex InChunkIndex, FChunkSaveData InChunkData);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FChunkSaveData LoadChunkData(FIndex InChunkIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<FVector, FChunkSaveData>& GetChunkDatas() { return ChunkDatas; }
};
