// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Starting.h"

#include "Audio/AudioModuleBPLibrary.h"
#include "Camera/CameraModuleBPLibrary.h"
#include "Camera/Roam/RoamCameraPawn.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Initializing.h"
#include "Procedure/Procedure_Pausing.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/DWVoxelModule.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"

UProcedure_Starting::UProcedure_Starting()
{
	ProcedureName = FName("Starting");
	ProcedureDisplayName = FText::FromString(TEXT("Starting"));
	
	BGMSound = nullptr;
}

#if WITH_EDITOR
void UProcedure_Starting::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Starting::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Starting::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Starting::OnEnter(UProcedureBase* InLastProcedure)
{
	UAudioModuleBPLibrary::PlaySingleSound2D(BGMSound, FName("BGM"));

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetMainMenu>();
	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetArchiveChoosingPanel>();

	if(!InLastProcedure || InLastProcedure->IsA<UProcedure_Initializing>())
	{
		USaveGameModuleBPLibrary::LoadOrCreateSaveGame<UDWArchiveSaveGame>();
	}
	else if(InLastProcedure->IsA<UProcedure_Pausing>())
	{
		AMainModule::GetModuleByClass<AVoxelModule>()->UnloadSaveData(false);
		UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->UnloadSaveData(false);
	}

	Super::OnEnter(InLastProcedure);
}

void UProcedure_Starting::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_Starting::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Starting::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
