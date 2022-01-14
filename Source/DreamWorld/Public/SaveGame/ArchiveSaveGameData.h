// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "SaveGame/SaveGameDataBase.h"

#include "ArchiveSaveGameData.generated.h"

UCLASS(DefaultToInstanced, Blueprintable)
class WHFRAMEWORK_API UArchiveSaveGameData : public USaveGameDataBase
{
	GENERATED_BODY()

public:
	UArchiveSaveGameData();

public:
	virtual void OnInitialize_Implementation() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FArchiveSaveData ArchiveData;
};
