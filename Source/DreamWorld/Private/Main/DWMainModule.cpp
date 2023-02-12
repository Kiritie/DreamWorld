// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Main/DWMainModule.h"

#include "Ability/AbilityModule.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "AI/AIModule.h"
#include "Asset/AssetModule.h"
#include "Audio/AudioModule.h"
#include "Camera/CameraModule.h"
#include "Camera/DWCameraModule.h"
#include "Character/CharacterModule.h"
#include "Character/DWCharacterModule.h"
#include "Debug/DebugModule.h"
#include "Event/EventModule.h"
#include "FSM/FSMModule.h"
#include "Input/InputModule.h"
#include "LatentAction/LatentActionModule.h"
#include "Media/MediaModule.h"
#include "Network/NetworkModule.h"
#include "ObjectPool/ObjectPoolModule.h"
#include "Parameter/ParameterModule.h"
#include "Procedure/ProcedureModule.h"
#include "SaveGame/DWSaveGameModule.h"
#include "Scene/SceneModule.h"
#include "ReferencePool/ReferencePoolModuleBPLibrary.h"
#include "Step/StepModule.h"
#include "Team/DWTeamModule.h"
#include "Voxel/DWVoxelModule.h"
#include "Voxel/Voxels/Voxel.h"
#include "WebRequest/WebRequestModule.h"
#include "Widget/WidgetModule.h"

MODULE_INSTANCE_IMPLEMENTATION(ADWMainModule, false)

ADWMainModule::ADWMainModule()
{
	// set default pawn class to our Blueprinted character

	ModuleClasses = TArray<TSubclassOf<AModuleBase>>();
	ModuleClasses.Add(AAbilityModule::StaticClass());
	ModuleClasses.Add(AAIModule::StaticClass());
	ModuleClasses.Add(AAssetModule::StaticClass());
	ModuleClasses.Add(AAudioModule::StaticClass());
	ModuleClasses.Add(ADWCameraModule::StaticClass());
	ModuleClasses.Add(ADWCharacterModule::StaticClass());
	ModuleClasses.Add(ADebugModule::StaticClass());
	ModuleClasses.Add(AEventModule::StaticClass());
	ModuleClasses.Add(AFSMModule::StaticClass());
	ModuleClasses.Add(AInputModule::StaticClass());
	ModuleClasses.Add(ALatentActionModule::StaticClass());
	ModuleClasses.Add(AMediaModule::StaticClass());
	ModuleClasses.Add(ANetworkModule::StaticClass());
	ModuleClasses.Add(AObjectPoolModule::StaticClass());
	ModuleClasses.Add(AParameterModule::StaticClass());
	ModuleClasses.Add(AProcedureModule::StaticClass());
	ModuleClasses.Add(AReferencePoolModule::StaticClass());
	ModuleClasses.Add(ADWSaveGameModule::StaticClass());
	ModuleClasses.Add(ASceneModule::StaticClass());
	ModuleClasses.Add(AStepModule::StaticClass());
	ModuleClasses.Add(ADWVoxelModule::StaticClass());
	ModuleClasses.Add(AWebRequestModule::StaticClass());
	ModuleClasses.Add(AWidgetModule::StaticClass());
	ModuleClasses.Add(ADWTeamModule::StaticClass());
}

void ADWMainModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();

	UAbilityModuleBPLibrary::AddCustomInteractAction((int32)EDWInteractAction::Feed, TEXT("EDWInteractAction"));
	UAbilityModuleBPLibrary::AddCustomInteractAction((int32)EDWInteractAction::Ride, TEXT("EDWInteractAction"));
	UAbilityModuleBPLibrary::AddCustomInteractAction((int32)EDWInteractAction::UnRide, TEXT("EDWInteractAction"));
}

