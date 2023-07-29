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

void ADWPlayerController::OnTermination_Implementation()
{
	Super::OnTermination_Implementation();
}

void ADWPlayerController::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWPlayerSaveData>();

	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	switch(InPhase)
	{
		case EPhase::Primary:
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
					if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Starting>())
					{
						PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, false);
					}
					SetPlayerPawn(PlayerCharacter);
				}
			}
			break;
		}
		case EPhase::Lesser:
		case EPhase::Final:
		{
			break;
		}
	}
	if(PlayerCharacter)
	{
		PlayerCharacter->LoadSaveData(&SaveData, InPhase, true);
	}
}

FSaveData* ADWPlayerController::ToData()
{
	return nullptr;
}

void ADWPlayerController::UnloadData(EPhase InPhase)
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	if(PlayerCharacter)
	{
		switch(InPhase)
		{
			case EPhase::Primary:
			{
				UObjectPoolModuleBPLibrary::DespawnObject(PlayerCharacter);
				SetPlayerPawn(nullptr);
				break;
			}
			case EPhase::Lesser:
			case EPhase::Final:
			{
				UnPossess();
				PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, false);
				break;
			}
		}
	}
}

void ADWPlayerController::OnPossess(APawn* InPawn)
{
	if(InPawn->IsA<ADWPlayerCharacter>() && (InPawn != GetPlayerPawn() || !GetPawn()))
	{
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGameHUD>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryBar>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryPanel>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGeneratePanel>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetShopPanel>(InPawn);
	}
	Super::OnPossess(InPawn);
}

void ADWPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	AMainModule::ResetModuleByClass<AInputModule>();
}

void ADWPlayerController::SetPlayerPawn(APawn* InPlayerPawn)
{
	if(InPlayerPawn->IsA<ADWPlayerCharacter>())
	{
		Super::SetPlayerPawn(InPlayerPawn);
	}
}
