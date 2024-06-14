// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/DWEventManager.h"
#include "Asset/AssetModuleStatics.h"

void UDWEventManager::OnInitialize()
{
	Super::OnInitialize();

	UAssetModuleStatics::AddEnumMapping(TEXT("/Script/WHFramework.EInteractAction"), TEXT("/Script/DreamWorld.EDWInteractAction"));
}
