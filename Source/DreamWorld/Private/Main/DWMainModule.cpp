// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Main/DWMainModule.h"

#include "Ability/AbilityModule.h"
#include "Ability/DWAbilityModule.h"
#include "Achievement/DWAchievementModule.h"
#include "AI/AIModule.h"
#include "Animation/AnimationModule.h"
#include "Asset/AssetModule.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Audio/DWAudioModule.h"
#include "Camera/DWCameraModule.h"
#include "Character/DWCharacterModule.h"
#include "Common/CommonBPLibrary.h"
#include "Debug/DebugModule.h"
#include "Event/EventModule.h"
#include "FSM/FSMModule.h"
#include "Input/DWInputModule.h"
#include "Network/NetworkModule.h"
#include "ObjectPool/ObjectPoolModule.h"
#include "Parameter/ParameterModule.h"
#include "Procedure/ProcedureModule.h"
#include "SaveGame/DWSaveGameModule.h"
#include "Scene/SceneModule.h"
#include "Step/StepModule.h"
#include "Task/DWTaskModule.h"
#include "Team/DWTeamModule.h"
#include "Video/DWVideoModule.h"
#include "Voxel/DWVoxelModule.h"
#include "WebRequest/WebRequestModule.h"
#include "Widget/WidgetModule.h"

IMPLEMENTATION_MAIN_MODULE(ADWMainModule)

ADWMainModule::ADWMainModule()
{
	// set default pawn class to our Blueprinted characterUCommonBPLibrary
	
	ModuleClasses = TArray<TSubclassOf<AModuleBase>>();
	ModuleClasses.Add(ADWAbilityModule::StaticClass());
	ModuleClasses.Add(ADWAchievementModule::StaticClass());
	ModuleClasses.Add(AAIModule::StaticClass());
	ModuleClasses.Add(AAnimationModule::StaticClass());
	ModuleClasses.Add(AAssetModule::StaticClass());
	ModuleClasses.Add(ADWAudioModule::StaticClass());
	ModuleClasses.Add(ADWCameraModule::StaticClass());
	ModuleClasses.Add(ADWCharacterModule::StaticClass());
	ModuleClasses.Add(ADebugModule::StaticClass());
	ModuleClasses.Add(AEventModule::StaticClass());
	ModuleClasses.Add(AFSMModule::StaticClass());
	ModuleClasses.Add(ADWInputModule::StaticClass());
	ModuleClasses.Add(ADWVideoModule::StaticClass());
	ModuleClasses.Add(ANetworkModule::StaticClass());
	ModuleClasses.Add(AObjectPoolModule::StaticClass());
	ModuleClasses.Add(AParameterModule::StaticClass());
	ModuleClasses.Add(AProcedureModule::StaticClass());
	ModuleClasses.Add(AReferencePoolModule::StaticClass());
	ModuleClasses.Add(ADWSaveGameModule::StaticClass());
	ModuleClasses.Add(ASceneModule::StaticClass());
	ModuleClasses.Add(AStepModule::StaticClass());
	ModuleClasses.Add(ADWTaskModule::StaticClass());
	ModuleClasses.Add(ADWVoxelModule::StaticClass());
	ModuleClasses.Add(AWebRequestModule::StaticClass());
	ModuleClasses.Add(AWidgetModule::StaticClass());
	ModuleClasses.Add(ADWTeamModule::StaticClass());
}

ADWMainModule::~ADWMainModule()
{
	TERMINATION_MAIN_MODULE(ADWMainModule)
}

void ADWMainModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();

	UAssetModuleBPLibrary::AddStaticObject(FName("EDWEquipPartType"), FStaticObject(UEnum::StaticClass(), TEXT("/Script/DreamWorld.EDWEquipPartType")));
	UAssetModuleBPLibrary::AddStaticObject(FName("EDWGameLevel"), FStaticObject(UEnum::StaticClass(), TEXT("/Script/DreamWorld.EDWGameLevel")));
	UAssetModuleBPLibrary::AddStaticObject(FName("EDWInventoryInitType"), FStaticObject(UEnum::StaticClass(), TEXT("/Script/DreamWorld.EDWInventoryInitType")));
}

void ADWMainModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

