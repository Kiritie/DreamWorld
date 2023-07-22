// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Loading.h"

#include "Character/CharacterModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "FSM/Components/FSMComponent.h"
#include "Gameplay/DWGameState.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Playing.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/DWVoxelModule.h"
#include "Voxel/VoxelModuleBPLibrary.h"
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

	UWidgetModuleBPLibrary::OpenUserWidget<UWidgetLoadingPanel>();
	
	UWidgetModuleBPLibrary::CreateUserWidget<UWidgetGameHUD>();

	if(ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>())
	{
		UCharacterModuleBPLibrary::SwitchCharacter(PlayerCharacter);
	}
	USaveGameModuleBPLibrary::LoadSaveGame<UDWArchiveSaveGame>(-1, EPhase::Final);
}

void UProcedure_Loading::OnRefresh()
{
	Super::OnRefresh();

	const ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>();
	if(PlayerCharacter && PlayerCharacter->GetFSMComponent()->IsCurrentStateClass<UDWCharacterState_Walk>())
	{
		UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Playing>();
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
