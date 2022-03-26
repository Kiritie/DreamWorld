// Fill out your copyright notice in the Description page of Project Settings.


#include "DWHelper.h"
#include "World/VoxelModule.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWPlayerController.h"
#include "Gameplay/DWGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Gameplay/DWGameInstance.h"
#include "Gameplay/DWGameState.h"
#include "Engine/DataTable.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerInput.h"
#include "Main/DWMainModule.h"
#include "Module/DWMainModule.h"
#include "Main/MainModuleBPLibrary.h"
#include "Widget/WidgetLoadingPanel.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetPausingMenu.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetSettingPanel.h"
#include "Widget/Archive/WidgetArchiveChoosingPanel.h"
#include "Widget/Archive/WidgetArchiveCreatingPanel.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryBox.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Widget/Other/WidgetWorldText.h"
#include "Widget/Other/WidgetVitalityHP.h"
#include "Widget/Other/WidgetCharacterHP.h"
#include "Widget/WidgetGameHUD.h"

ADWGameState* UDWHelper::CurrentGameState = nullptr;

ADWGameMode* UDWHelper::CurrentGameMode = nullptr;

UDWGameInstance* UDWHelper::CurrentGameInstance = nullptr;

ADWPlayerController* UDWHelper::CurrentPlayerController = nullptr;

UDWHelper::UDWHelper(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	LoadItemDatas();
}

ADWGameMode* UDWHelper::GetGameMode(const UObject* InWorldContext)
{
	if ((!CurrentGameMode || !CurrentGameMode->IsValidLowLevel()) && (InWorldContext && InWorldContext->IsValidLowLevel()))
	{
		CurrentGameMode = Cast<ADWGameMode>(UGameplayStatics::GetGameMode(InWorldContext));
	}
	return CurrentGameMode;
}

ADWGameState* UDWHelper::GetGameState(const UObject* InWorldContext)
{
	if ((!CurrentGameState || !CurrentGameState->IsValidLowLevel()) && (InWorldContext && InWorldContext->IsValidLowLevel()))
	{
		CurrentGameState = Cast<ADWGameState>(UGameplayStatics::GetGameState(InWorldContext));
	}
	return CurrentGameState;
}

UDWGameInstance* UDWHelper::GetGameInstance(const UObject* InWorldContext)
{
	if ((!CurrentGameInstance || !CurrentGameInstance->IsValidLowLevel()) && (InWorldContext && InWorldContext->IsValidLowLevel()))
	{
		CurrentGameInstance = Cast<UDWGameInstance>(UGameplayStatics::GetGameInstance(InWorldContext));
	}
	return CurrentGameInstance;
}

ADWPlayerController* UDWHelper::GetPlayerController(const UObject* InWorldContext)
{
	if ((!CurrentPlayerController || !CurrentPlayerController->IsValidLowLevel()) && (InWorldContext && InWorldContext->IsValidLowLevel()))
	{
		CurrentPlayerController = Cast<ADWPlayerController>(UGameplayStatics::GetPlayerController(InWorldContext, 0));
	}
	return CurrentPlayerController;
}

ADWPlayerCharacter* UDWHelper::GetPlayerCharacter(const UObject* InWorldContext)
{
	if (ADWPlayerController* PlayerController = GetPlayerController(InWorldContext))
	{
		return PlayerController->GetPlayerCharacter();
	}
	return nullptr;
}

ADWMainModule* UDWHelper::GetMainModule()
{
	return Cast<ADWMainModule>(UMainModuleBPLibrary::GetMainModule());
}

FVitalityRaceData UDWHelper::RandomVitalityRaceData()
{
	auto raceDatas = UDWHelper::LoadVitalityRaceDatas();
	float tmpNum1 = 0;
	float tmpNum2 = 0;
	for (int32 i = 0; i < raceDatas.Num(); i++)
	{
		tmpNum1 += raceDatas[i].Proportion;
	}
	tmpNum1 = FMath::FRandRange(0, tmpNum1);
	for (int32 i = 0; i < raceDatas.Num(); i++)
	{
		tmpNum2 += raceDatas[i].Proportion;
		if (tmpNum1 <= tmpNum2)
		{
			return raceDatas[i];
		}
	}
	return FVitalityRaceData();
}

FCharacterRaceData UDWHelper::RandomCharacterRaceData()
{
	auto RaceDatas = UDWHelper::LoadCharacterRaceDatas();
	float TmpNum1 = 0;
	float TmpNum2 = 0;
	
	for (int32 i = 0; i < RaceDatas.Num(); i++)
	{
		TmpNum1 += RaceDatas[i].Proportion;
	}
	TmpNum1 = FMath::FRandRange(0, TmpNum1);
	for (int32 i = 0; i < RaceDatas.Num(); i++)
	{
		TmpNum2 += RaceDatas[i].Proportion;
		if (TmpNum1 <= TmpNum2)
		{
			return RaceDatas[i];
		}
	}
	return FCharacterRaceData();
}

ETraceTypeQuery UDWHelper::GetGameTrace(EGameTraceType InGameTraceType)
{
	return UEngineTypes::ConvertToTraceType((ECollisionChannel)InGameTraceType);
}
