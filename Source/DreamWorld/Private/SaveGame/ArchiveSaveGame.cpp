// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/ArchiveSaveGame.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "World/WorldManager.h"

UArchiveSaveGame::UArchiveSaveGame()
{
	// set default pawn class to our Blueprinted character
	
	ArchiveData = FArchiveSaveData();
	ChunkDatas = TMap<FVector, FChunkSaveData>();
}

void UArchiveSaveGame::RefreshSaveData_Implementation()
{
	if(ADWPlayerCharacter* PlayerCharacter = UDWHelper::GetPlayerCharacter(this))
	{
		ArchiveData.PlayerData = *static_cast<FPlayerSaveData*>(PlayerCharacter->ToData());
	}
	if(AWorldManager* WorldManager = AWorldManager::Get())
	{
		ArchiveData.WorldData = WorldManager->GetWorldData();
		ArchiveData.WorldData.bSaved = true;
	}
}

bool UArchiveSaveGame::IsExistChunkData(FIndex InChunkIndex)
{
	return ChunkDatas.Contains(InChunkIndex.ToVector());
}

void UArchiveSaveGame::SaveChunkData(FIndex InChunkIndex, FChunkSaveData InChunkData)
{
	if (!ChunkDatas.Contains(InChunkIndex.ToVector()))
		ChunkDatas.Add(InChunkIndex.ToVector(), InChunkData);
	else
		ChunkDatas[InChunkIndex.ToVector()] = InChunkData;
}

FChunkSaveData UArchiveSaveGame::LoadChunkData(FIndex InChunkIndex)
{
	if (ChunkDatas.Contains(InChunkIndex.ToVector()))
		return ChunkDatas[InChunkIndex.ToVector()];
	return FChunkSaveData();
}
