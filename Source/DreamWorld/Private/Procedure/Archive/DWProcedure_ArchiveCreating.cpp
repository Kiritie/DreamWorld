// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Archive/DWProcedure_ArchiveCreating.h"

#include "Camera/CameraModule.h"
#include "Character/DWCharacterData.h"
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
#include "Kismet/KismetMathLibrary.h"
#include "Procedure/Archive/DWProcedure_ArchiveChoosing.h"

UDWProcedure_ArchiveCreating::UDWProcedure_ArchiveCreating()
{
	ProcedureName = FName("ArchiveCreating");
	ProcedureDisplayName = FText::FromString(TEXT("创建存档"));

	bTrackTarget = true;
	TrackTargetMode = ECameraTrackMode::LocationOnly;

	MinCameraDistance = 60.f;
	MaxCameraDistance = 350.f;

	LocalMinCameraDistance = 0.f;
	LocalMaxCameraDistance = 0.f;
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

	if(ADWPlayerCharacter* PlayerCharacter = GetOperationTarget<ADWPlayerCharacter>())
	{
		PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, true);
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

	LocalMinCameraDistance = UCameraModule::Get().GetMinCameraDistance();
	LocalMaxCameraDistance = UCameraModule::Get().GetMaxCameraDistance();
	
	UCameraModule::Get().SetMinCameraDistance(MinCameraDistance);
	UCameraModule::Get().SetMaxCameraDistance(MaxCameraDistance);
}

void UDWProcedure_ArchiveCreating::OnRefresh()
{
	Super::OnRefresh();

	if(ADWPlayerCharacter* PlayerCharacter = GetOperationTarget<ADWPlayerCharacter>())
	{
		UCameraModule::Get().SetCameraOffset(UKismetMathLibrary::VLerp(FVector::ZeroVector, PlayerCharacter->GetCharacterData<UDWCharacterData>().EyesOffset, 1.f - FMath::Clamp(UCameraModule::Get().GetCurrentCameraDistance(true) - UCameraModule::Get().GetMinCameraDistance(), 0.f, 100.f) / 100.f));
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
		if(ADWPlayerCharacter* PlayerCharacter = GetOperationTarget<ADWPlayerCharacter>())
		{
			PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, false);
		}
	}

	UCameraModule::Get().SetMinCameraDistance(LocalMinCameraDistance);
	UCameraModule::Get().SetMaxCameraDistance(LocalMaxCameraDistance);
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
	SaveData.InitData();
	return SaveData;
}

FDWWorldSaveData& UDWProcedure_ArchiveCreating::GetWorldData() const
{
	static FDWWorldSaveData SaveData;
	SaveData = WorldSaveData;
	return SaveData;
}
