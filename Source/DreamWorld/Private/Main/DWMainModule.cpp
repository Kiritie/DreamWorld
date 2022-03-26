// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Module/DWMainModule.h"

#include "Asset/AssetModule.h"
#include "Audio/AudioModule.h"
#include "Camera/CameraModule.h"
#include "Character/CharacterModule.h"
#include "Debug/DebugModule.h"
#include "Event/EventModule.h"
#include "Input/InputModule.h"
#include "LatentAction/LatentActionModule.h"
#include "Media/MediaModule.h"
#include "Module/DWSaveGameModule.h"
#include "Network/NetworkModule.h"
#include "ObjectPool/ObjectPoolModule.h"
#include "Parameter/ParameterModule.h"
#include "Procedure/ProcedureModule.h"
#include "Scene/SceneModule.h"
#include "SpawnPool/SpawnPoolModuleBPLibrary.h"
#include "Voxel/Voxel.h"
#include "WebRequest/WebRequestModule.h"
#include "Widget/WidgetModule.h"
#include "World/Chunk.h"

ADWMainModule::ADWMainModule()
{
	// set default pawn class to our Blueprinted character

	ModuleClasses = TArray<TSubclassOf<AModuleBase>>();
	ModuleClasses.Add(AAssetModule::StaticClass());
	ModuleClasses.Add(AAudioModule::StaticClass());
	ModuleClasses.Add(ACharacterModule::StaticClass());
	ModuleClasses.Add(ACameraModule::StaticClass());
	ModuleClasses.Add(ADebugModule::StaticClass());
	ModuleClasses.Add(AEventModule::StaticClass());
	ModuleClasses.Add(AInputModule::StaticClass());
	ModuleClasses.Add(ALatentActionModule::StaticClass());
	ModuleClasses.Add(AMediaModule::StaticClass());
	ModuleClasses.Add(ANetworkModule::StaticClass());
	ModuleClasses.Add(AObjectPoolModule::StaticClass());
	ModuleClasses.Add(AParameterModule::StaticClass());
	ModuleClasses.Add(AProcedureModule::StaticClass());
	ModuleClasses.Add(ADWSaveGameModule::StaticClass());
	ModuleClasses.Add(ASceneModule::StaticClass());
	ModuleClasses.Add(ASpawnPoolModule::StaticClass());
	ModuleClasses.Add(AWebRequestModule::StaticClass());
	ModuleClasses.Add(AWidgetModule::StaticClass());
}

void ADWMainModule::InitializeModules_Implementation()
{
	Super::InitializeModules_Implementation();
	
	UVoxel::EmptyVoxel = UVoxel::SpawnVoxel(EVoxelType::Empty);
	UVoxel::UnknownVoxel = UVoxel::SpawnVoxel(EVoxelType::Unknown);
	
	USpawnPoolModuleBPLibrary::DespawnActor(USpawnPoolModuleBPLibrary::SpawnActor<AVoxelChunk>());
}
