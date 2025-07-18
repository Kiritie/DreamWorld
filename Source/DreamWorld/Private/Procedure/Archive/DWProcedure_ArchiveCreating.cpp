// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Archive/DWProcedure_ArchiveCreating.h"

#include "Camera/CameraModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWPlayerController.h"
#include "Common/CommonStatics.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/DWProcedure_Loading.h"
#include "SaveGame/Archive/DWArchiveSaveGame.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"
#include "Voxel/VoxelModule.h"
#include "Common/DWCommonTypes.h"
#include "Procedure/Archive/DWProcedure_ArchiveChoosing.h"

UDWProcedure_ArchiveCreating::UDWProcedure_ArchiveCreating()
{
	ProcedureName = FName("ArchiveCreating");
	ProcedureDisplayName = FText::FromString(TEXT("创建存档"));

	bTrackTarget = true;
	TrackTargetMode = ECameraTrackMode::LocationOnly;
}

#if WITH_EDITOR
void UDWProcedure_ArchiveCreating::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_ArchiveCreating::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_ArchiveCreating::OnInitialize()
{
	Super::OnInitialize();

	UCommonStatics::GetPlayerController<ADWPlayerController>()->OnPlayerPawnChanged.AddDynamic(this, &UDWProcedure_ArchiveCreating::OnPlayerChanged);
}

void UDWProcedure_ArchiveCreating::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	if(OperationTarget.LoadSynchronous() && OperationTarget.LoadSynchronous()->Implements<USceneActorInterface>())
	{
		ISceneActorInterface::Execute_SetActorVisible(OperationTarget.LoadSynchronous(), true);
	}

	if(!InLastProcedure)
	{
		if(USaveGameModuleStatics::GetSaveGameInfo<UDWArchiveSaveGame>().ActiveIndex == -1)
		{
			USaveGameModuleStatics::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::Primary);
			CreatePlayer(GetPlayerData(), EPhase::Lesser);
			CreateWorld(GetWorldData(), EPhase::Lesser);
			CreateArchive(USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->GetSaveDataRef<FDWArchiveSaveData>());
		}
		else
		{
			USaveGameModuleStatics::LoadOrCreateSaveGame<UDWArchiveSaveGame>(-1, EPhase::Primary);
			UProcedureModuleStatics::SwitchProcedureByClass<UDWProcedure_Loading>();
		}
	}
	else
	{
		UWidgetModuleStatics::OpenUserWidget<UWidgetArchiveCreatingPanel>();
	}
}

void UDWProcedure_ArchiveCreating::OnRefresh()
{
	Super::OnRefresh();

	if(ADWPlayerCharacter* PlayerCharacter = Cast<ADWPlayerCharacter>(OperationTarget.LoadSynchronous()))
	{
		WHDebug(FString::SanitizeFloat(UCameraModule::Get().GetCurrentCameraDistance() - UCameraModule::Get().GetMinCameraDistance()));
		UCameraModule::Get().SetCameraOffset(FVector(0.f, 0.f, FMath::Lerp(0.f, PlayerCharacter->GetHalfHeight() - 20.f, 1.f - 100.f / FMath::Clamp(UCameraModule::Get().GetCurrentCameraDistance() - UCameraModule::Get().GetMinCameraDistance(), 0.f, 100.f))));
	}
}

void UDWProcedure_ArchiveCreating::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_ArchiveCreating::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);

	if(InNextProcedure->IsA<UDWProcedure_ArchiveChoosing>())
	{
		if(OperationTarget.LoadSynchronous() && OperationTarget.LoadSynchronous()->Implements<USceneActorInterface>())
		{
			ISceneActorInterface::Execute_SetActorVisible(OperationTarget.LoadSynchronous(), false);
		}
	}
}

void UDWProcedure_ArchiveCreating::OnPlayerChanged(APawn* InPlayerPawn)
{
	SetOperationTarget(InPlayerPawn);
}

void UDWProcedure_ArchiveCreating::CreatePlayer(FDWPlayerSaveData& InPlayerSaveData, EPhase InPhase)
{
	if(!IsCurrent() || !InPlayerSaveData.IsValid()) return;
	
	UCommonStatics::GetPlayerController<ADWPlayerController>()->LoadSaveData(&InPlayerSaveData, InPhase);
	USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->GetSaveDataRef<FDWArchiveSaveData>().PlayerData = InPlayerSaveData;
}

void UDWProcedure_ArchiveCreating::CreateWorld(FDWWorldSaveData& InWorldSaveData, EPhase InPhase)
{
	if(!IsCurrent()) return;
	
	UVoxelModule::Get().LoadSaveData(&InWorldSaveData, InPhase);
	USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->GetSaveDataRef<FDWArchiveSaveData>().WorldData = InWorldSaveData;
}

void UDWProcedure_ArchiveCreating::CreateArchive(FDWArchiveSaveData& InArchiveSaveData)
{
	if(!IsCurrent()) return;
	
	USaveGameModuleStatics::GetSaveGame<UDWArchiveSaveGame>()->SetSaveData(&InArchiveSaveData);
	UProcedureModuleStatics::SwitchProcedureByClass<UDWProcedure_Loading>();
}

FDWPlayerSaveData& UDWProcedure_ArchiveCreating::GetPlayerData() const
{
	static FDWPlayerSaveData SaveData;
	SaveData = PlayerSaveData;
	SaveData.InitTalentData();
	SaveData.InitInventoryData();
	return SaveData;
}

FDWWorldSaveData& UDWProcedure_ArchiveCreating::GetWorldData() const
{
	static FDWWorldSaveData SaveData;
	SaveData = WorldSaveData;
	return SaveData;
}
