// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SaveGame/SaveGameArchive.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "World/WorldManager.h"

USaveGameArchive::USaveGameArchive()
{
	// set default pawn class to our Blueprinted character
	
	ArchiveData = FArchiveSaveData();
	ChunkDatas = TMap<FVector, FChunkSaveData>();
}

void USaveGameArchive::RefreshSaveData_Implementation()
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

bool USaveGameArchive::IsExistChunkData(FIndex InChunkIndex)
{
	return ChunkDatas.Contains(InChunkIndex.ToVector());
}

void USaveGameArchive::SaveChunkData(FIndex InChunkIndex, FChunkSaveData InChunkData)
{
	if (!ChunkDatas.Contains(InChunkIndex.ToVector()))
		ChunkDatas.Add(InChunkIndex.ToVector(), InChunkData);
	else
		ChunkDatas[InChunkIndex.ToVector()] = InChunkData;
}

FChunkSaveData USaveGameArchive::LoadChunkData(FIndex InChunkIndex)
{
	if (ChunkDatas.Contains(InChunkIndex.ToVector()))
		return ChunkDatas[InChunkIndex.ToVector()];
	return FChunkSaveData();
}
