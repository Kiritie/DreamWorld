// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DWInputModule.h"

#include "Camera/CameraModule.h"
#include "Common/CommonStatics.h"
#include "Input/InputModule.h"
#include "Input/InputModuleStatics.h"
#include "Common/DWCommonTypes.h"
#include "Input/DWInputManager.h"

IMPLEMENTATION_MODULE(UDWInputModule)

// ParamSets default values
UDWInputModule::UDWInputModule()
{
	InputManagers.Empty();
	InputManagers.Add(UDWInputManager::StaticClass());

	AddKeyShortcut(GameplayTags::Input_InventoryAll, FInputKeyShortcut(FText::FromString("Inventory All"), FText::FromString("Inventory")));
	AddKeyShortcut(GameplayTags::Input_InventorySingle, FInputKeyShortcut(FText::FromString("Inventory Single"), FText::FromString("Inventory")));
	AddKeyShortcut(GameplayTags::Input_InventorySplit, FInputKeyShortcut(FText::FromString("Inventory Split"), FText::FromString("Inventory")));
}

UDWInputModule::~UDWInputModule()
{
	TERMINATION_MODULE(UDWInputModule)
}

#if WITH_EDITOR
void UDWInputModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWInputModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWInputModule)
}
#endif

void UDWInputModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWInputModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWInputModule::OnRefresh(float DeltaSeconds, bool bInEditor)
{
	Super::OnRefresh(DeltaSeconds, bInEditor);
}

void UDWInputModule::OnReset()
{
	Super::OnReset();
}

void UDWInputModule::OnPause()
{
	Super::OnPause();
}

void UDWInputModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWInputModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWInputModuleSaveData>();
}

void UDWInputModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* UDWInputModule::ToData()
{
	static FDWInputModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FInputModuleSaveData>();

	return &SaveData;
}
