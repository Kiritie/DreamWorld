// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/DWEventManager.h"
#include "Asset/AssetModuleStatics.h"
#include "Common/DWCommonTypes.h"
#include "Scene/SceneModuleStatics.h"

void UDWEventManager::OnInitialize()
{
	Super::OnInitialize();

	UAssetModuleStatics::AddEnumMapping(TEXT("/Script/WHFramework.EInteractAction"), TEXT("/Script/DreamWorld.EDWInteractAction"));

	UAssetModuleStatics::AddStaticObject(FName("EDWEquipPart"), FStaticObject(UEnum::StaticClass(), TEXT("/Script/DreamWorld.EDWEquipPart")));
	UAssetModuleStatics::AddStaticObject(FName("EDWGameLevel"), FStaticObject(UEnum::StaticClass(), TEXT("/Script/DreamWorld.EDWGameLevel")));
	UAssetModuleStatics::AddStaticObject(FName("EDWInventoryInitType"), FStaticObject(UEnum::StaticClass(), TEXT("/Script/DreamWorld.EDWInventoryInitType")));

	USceneModuleStatics::AddTraceMapping(FName("PickUp"), (ECollisionChannel)EGameTraceChannel::Chunk);
	USceneModuleStatics::AddTraceMapping(FName("Sight"), (ECollisionChannel)EDWGameTraceChannel::Sight);
	USceneModuleStatics::AddTraceMapping(FName("Step"), (ECollisionChannel)EDWGameTraceChannel::Step);
}
