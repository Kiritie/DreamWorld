// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Main/DWMainModule.h"

#include "Ability/AbilityModule.h"
#include "Ability/DWAbilityModule.h"
#include "AI/AIModule.h"
#include "Animation/AnimationModule.h"
#include "Asset/AssetModule.h"
#include "Audio/AudioModule.h"
#include "Camera/CameraModule.h"
#include "Camera/DWCameraModule.h"
#include "Character/CharacterModule.h"
#include "Character/DWCharacterModule.h"
#include "Debug/DebugModule.h"
#include "Event/EventModule.h"
#include "FSM/FSMModule.h"
#include "Input/DWInputModule.h"
#include "Input/InputModule.h"
#include "Media/MediaModule.h"
#include "Network/NetworkModule.h"
#include "ObjectPool/ObjectPoolModule.h"
#include "Parameter/ParameterModule.h"
#include "Procedure/ProcedureModule.h"
#include "SaveGame/DWSaveGameModule.h"
#include "Scene/SceneModule.h"
#include "Step/StepModule.h"
#include "Task/TaskModule.h"
#include "Team/DWTeamModule.h"
#include "Voxel/DWVoxelModule.h"
#include "Voxel/Voxels/Voxel.h"
#include "WebRequest/WebRequestModule.h"
#include "Widget/WidgetModule.h"

IMPLEMENTATION_MAIN_MODULE(ADWMainModule)

ADWMainModule::ADWMainModule()
{
	// set default pawn class to our Blueprinted character

	ModuleClasses = TArray<TSubclassOf<AModuleBase>>();
	ModuleClasses.Add(ADWAbilityModule::StaticClass());
	ModuleClasses.Add(AAIModule::StaticClass());
	ModuleClasses.Add(AAnimationModule::StaticClass());
	ModuleClasses.Add(AAssetModule::StaticClass());
	ModuleClasses.Add(AAudioModule::StaticClass());
	ModuleClasses.Add(ADWCameraModule::StaticClass());
	ModuleClasses.Add(ADWCharacterModule::StaticClass());
	ModuleClasses.Add(ADebugModule::StaticClass());
	ModuleClasses.Add(AEventModule::StaticClass());
	ModuleClasses.Add(AFSMModule::StaticClass());
	ModuleClasses.Add(ADWInputModule::StaticClass());
	ModuleClasses.Add(AMediaModule::StaticClass());
	ModuleClasses.Add(ANetworkModule::StaticClass());
	ModuleClasses.Add(AObjectPoolModule::StaticClass());
	ModuleClasses.Add(AParameterModule::StaticClass());
	ModuleClasses.Add(AProcedureModule::StaticClass());
	ModuleClasses.Add(AReferencePoolModule::StaticClass());
	ModuleClasses.Add(ADWSaveGameModule::StaticClass());
	ModuleClasses.Add(ASceneModule::StaticClass());
	ModuleClasses.Add(AStepModule::StaticClass());
	ModuleClasses.Add(ATaskModule::StaticClass());
	ModuleClasses.Add(ADWVoxelModule::StaticClass());
	ModuleClasses.Add(AWebRequestModule::StaticClass());
	ModuleClasses.Add(AWidgetModule::StaticClass());
	ModuleClasses.Add(ADWTeamModule::StaticClass());
}

ADWMainModule::~ADWMainModule()
{
	TERMINATION_MAIN_MODULE(ADWMainModule)
}

