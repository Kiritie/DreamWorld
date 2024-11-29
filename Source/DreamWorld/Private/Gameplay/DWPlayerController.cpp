// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DWPlayerController.h"

#include "Ability/Actor/AbilityActorBase.h"
#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Camera/CameraModuleStatics.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/DWCharacterData.h"
#include "Common/CommonStatics.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Procedure/Archive/DWProcedure_ArchiveCreating.h"
#include "Character/CharacterModuleStatics.h"
#include "Input/InputModule.h"
#include "Main/MainModule.h"
#include "Voxel/VoxelModuleStatics.h"

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
			SaveData.SpawnTransform = PlayerCharacter->GetActorTransform();
			if(PlayerCharacter->Execute_GetAssetID(PlayerCharacter) == SaveData.AssetID)
			{
				bNeedSpawn = false;
			}
		}
		if(bNeedSpawn)
		{
			UnloadData(EPhase::Primary);
			PlayerCharacter = UObjectPoolModuleStatics::SpawnObject<ADWPlayerCharacter>(nullptr, { SaveData.ActorID, SaveData.AssetID }, SaveData.GetData<UDWCharacterData>().Class);
			SetPlayerPawn(PlayerCharacter);
			PlayerCharacter->LoadSaveData(&SaveData, EPhase::Primary);
			PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, UProcedureModuleStatics::IsCurrentProcedureClass<UDWProcedure_ArchiveCreating>());
			PlayerCharacter->DisableInput(nullptr);
		}
	}
	if(PHASEC(InPhase, EPhase::Lesser))
	{
		PlayerCharacter->LoadSaveData(&SaveData, EPhase::Lesser);
	}
	if(PHASEC(InPhase, EPhase::Final))
	{
		PlayerCharacter->LoadSaveData(&SaveData, EPhase::Final);
		PlayerCharacter->EnableInput(nullptr);
		UCharacterModuleStatics::SwitchCharacter(PlayerCharacter, true);
		if(SaveData.IsSaved())
		{
			UCameraModuleStatics::SetCameraRotation(SaveData.CameraRotation.Yaw, SaveData.CameraRotation.Pitch, true);
		}
		UCameraModuleStatics::SetCameraDistance(SaveData.CameraDistance, true);
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
		if(PHASEC(InPhase, EPhase::Primary))
		{
			UObjectPoolModuleStatics::DespawnObject(PlayerCharacter);
			SetPlayerPawn(nullptr);
			UCharacterModuleStatics::SwitchCharacter(nullptr);
		}
		if(PHASEC(InPhase, EPhase::Lesser))
		{
			PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, false);
			PlayerCharacter->DisableInput(nullptr);
			UCharacterModuleStatics::SwitchCharacter(nullptr);
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

	AMainModule::ResetModuleByClass<UInputModule>();
}

void ADWPlayerController::SetPlayerPawn(APawn* InPlayerPawn)
{
	if(InPlayerPawn && !InPlayerPawn->IsA<ADWPlayerCharacter>()) return;

	Super::SetPlayerPawn(InPlayerPawn);
}
