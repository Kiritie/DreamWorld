// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DWPlayerController.h"

#include "Engine/World.h"
#include "Engine.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Ability/Components/CharacterInteractionComponent.h"
#include "Ability/Components/InteractionComponent.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Camera/CameraModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Static.h"
#include "FSM/Components/FSMComponent.h"
#include "Widget/WidgetGameHUD.h"
#include "Gameplay/DWGameInstance.h"
#include "Gameplay/DWGameState.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Gameplay/DWGameMode.h"
#include "Global/GlobalBPLibrary.h"
#include "Input/InputModuleBPLibrary.h"
#include "Ability/Inventory/Slot/InventorySlot.h"
#include "Ability/Inventory/CharacterInventory.h"
#include "Ability/Inventory/Slot/InventorySkillSlot.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Pausing.h"
#include "Procedure/Procedure_Playing.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Datas/VoxelData.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Voxels/Voxel.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Camera/CameraModule.h"
#include "Widget/WidgetGeneratePanel.h"
#include "Widget/WidgetShopPanel.h"
#include "Procedure/Procedure_Starting.h"
#include "Procedure/Archive/Procedure_ArchiveCreating.h"
#include "Character/CharacterModuleBPLibrary.h"

ADWPlayerController::ADWPlayerController()
{
	
}

void ADWPlayerController::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWPlayerController::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWPlayerController::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWPlayerController::OnTermination_Implementation(EPhase InPhase)
{
	Super::OnTermination_Implementation(InPhase);
}

void ADWPlayerController::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWPlayerSaveData>();

	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(PHASEC(InPhase, EPhase::Primary))
	{
		bool bNeedSpawn = true;
		if(PlayerCharacter)
		{
			if(PlayerCharacter->GetAssetID() == SaveData.ID)
			{
				bNeedSpawn = false;
			}
			SaveData.SpawnLocation = PlayerCharacter->GetActorLocation();
		}
		if(bNeedSpawn)
		{
			UnloadData(InPhase);
			PlayerCharacter = UObjectPoolModuleBPLibrary::SpawnObject<ADWPlayerCharacter>({ &SaveData.ID }, SaveData.GetCharacterData().Class);
			if(PlayerCharacter)
			{
				SetPlayerPawn(PlayerCharacter);
				PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_ArchiveCreating>());
			}
		}
	}
	if(PHASEC(InPhase, EPhase::All))
	{
		if(PlayerCharacter)
		{
			PlayerCharacter->LoadSaveData(&SaveData, InPhase);
		}
	}
	if(PHASEC(InPhase, EPhase::Final))
	{
		UCharacterModuleBPLibrary::SwitchCharacter(PlayerCharacter, true, true);
		UCameraModuleBPLibrary::SetCameraRotationAndDistance(SaveData.CameraRotation.Yaw, SaveData.CameraRotation.Pitch, SaveData.CameraDistance, true);
	}
}

FSaveData* ADWPlayerController::ToData(bool bRefresh)
{
	return nullptr;
}

void ADWPlayerController::UnloadData(EPhase InPhase)
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(PlayerCharacter)
	{
		if(PHASEC(InPhase, EPhase::PrimaryAndFinal))
		{
			UObjectPoolModuleBPLibrary::DespawnObject(PlayerCharacter);
			SetPlayerPawn(nullptr);
		}
		if(PHASEC(InPhase, EPhase::Lesser))
		{
			PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, false);
		}
	}
}

void ADWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ADWPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	AMainModule::ResetModuleByClass<AInputModule>();
}

void ADWPlayerController::SetPlayerPawn(APawn* InPlayerPawn)
{
	if(InPlayerPawn && !InPlayerPawn->IsA<ADWPlayerCharacter>()) return;

	Super::SetPlayerPawn(InPlayerPawn);
}
