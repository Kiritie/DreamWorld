// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/DWEventManager.h"
#include "Asset/AssetModuleStatics.h"
#include "Common/DWCommonTypes.h"
#include "Scene/SceneModuleStatics.h"

void UDWEventManager::OnInitialize()
{
	Super::OnInitialize();

	UAssetModuleStatics::AddEnumMapping(TEXT("/Script/WHFramework.EInteractAction"), TEXT("/Script/DreamWorld.EDWInteractAction"));
	
	USceneModuleStatics::AddTraceMapping(FName("Chunk"), (ECollisionChannel)EDWGameTraceChannel::Chunk);
	USceneModuleStatics::AddTraceMapping(FName("Voxel"), (ECollisionChannel)EDWGameTraceChannel::Voxel);

	USceneModuleStatics::AddTraceMapping(FName("Sight"), (ECollisionChannel)EDWGameTraceChannel::Sight);
	USceneModuleStatics::AddTraceMapping(FName("Step"), (ECollisionChannel)EDWGameTraceChannel::Step);
}
