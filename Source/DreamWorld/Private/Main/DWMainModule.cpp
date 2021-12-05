// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Main/DWMainModule.h"

#include "SpawnPool/SpawnPoolModuleBPLibrary.h"
#include "Voxel/Voxel.h"
#include "World/Chunk.h"

ADWMainModule::ADWMainModule()
{
	// set default pawn class to our Blueprinted character
	
}

void ADWMainModule::InitializeModules_Implementation()
{
	Super::InitializeModules_Implementation();
	
	UVoxel::EmptyVoxel = UVoxel::SpawnVoxel(EVoxelType::Empty);
	UVoxel::UnknownVoxel = UVoxel::SpawnVoxel(EVoxelType::Unknown);
	
	USpawnPoolModuleBPLibrary::DespawnActor(USpawnPoolModuleBPLibrary::SpawnActor<AChunk>());
}
