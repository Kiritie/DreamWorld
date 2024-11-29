// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Main/DWMainModule.h"

#include "Asset/AssetModuleStatics.h"
#include "Common/CommonStatics.h"
#include "Common/DWCommonTypes.h"
#include "Scene/SceneModuleStatics.h"

IMPLEMENTATION_MAIN_MODULE(ADWMainModule)

ADWMainModule::ADWMainModule()
{
}

ADWMainModule::~ADWMainModule()
{
	TERMINATION_MAIN_MODULE(ADWMainModule)
}

void ADWMainModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();

	USceneModuleStatics::AddTraceMapping(FName("PickUp"), (ECollisionChannel)EDWGameTraceChannel::Chunk);
	UAssetModuleStatics::AddStaticObject(FName("EDWEquipPart"), FStaticObject(UEnum::StaticClass(), TEXT("/Script/DreamWorld.EDWEquipPart")));
	UAssetModuleStatics::AddStaticObject(FName("EDWGameLevel"), FStaticObject(UEnum::StaticClass(), TEXT("/Script/DreamWorld.EDWGameLevel")));
	UAssetModuleStatics::AddStaticObject(FName("EDWInventoryInitType"), FStaticObject(UEnum::StaticClass(), TEXT("/Script/DreamWorld.EDWInventoryInitType")));
}

void ADWMainModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}
