// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Loading.h"

#include "Character/CharacterModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Playing.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/DWVoxelModule.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetLoadingPanel.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"

UProcedure_Loading::UProcedure_Loading()
{
	ProcedureName = FName("Loading");
	ProcedureDisplayName = FText::FromString(TEXT("Loading"));
}

#if WITH_EDITOR
void UProcedure_Loading::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Loading::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Loading::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Loading::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	UGlobalBPLibrary::GetGameState<ADWGameState>()->SetCurrentState(EDWGameState::Loading);

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetLoadingPanel>();
	
	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetGameHUD>();
	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetInventoryBar>();
	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetInventoryPanel>();

	AMainModule::GetModuleByClass<ADWVoxelModule>()->SetWorldMode(EVoxelWorldMode::Normal);

	USaveGameModuleBPLibrary::LoadSaveGame<UDWArchiveSaveGame>(USaveGameModuleBPLibrary::LoadSaveGame<UDWGeneralSaveGame>()->GetCurrentArchiveID());
}

void UProcedure_Loading::OnRefresh()
{
	Super::OnRefresh();

	if(ADWVoxelModule* VoxelModule = AMainModule::GetModuleByClass<ADWVoxelModule>())
	{
		switch(VoxelModule->GetWorldState())
		{
			case EVoxelWorldState::BasicGenerated:
			case EVoxelWorldState::FullGenerated:
			{
				UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Playing>();
				break;
			}
			default: break;
		}
	}
}

void UProcedure_Loading::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Loading::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);

	UWidgetModuleBPLibrary::CloseUserWidget<UWidgetLoadingPanel>();
}
