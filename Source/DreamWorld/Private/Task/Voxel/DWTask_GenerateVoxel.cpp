// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Voxel/DWTask_GenerateVoxel.h"

#include "TimerManager.h"
#include "Event/EventModuleStatics.h"
#include "Common/CommonStatics.h"
#include "Event/Handle/Voxel/EventHandle_VoxelGenerated.h"
#include "Voxel/Agent/VoxelAgentInterface.h"

UDWTask_GenerateVoxel::UDWTask_GenerateVoxel()
{
	TaskDisplayName = FText::FromString(TEXT("建造方块"));
	TaskDescription = FText::FromString(TEXT("建造XX个方块"));

	TargetVoxelID = FPrimaryAssetId();
	TargetCount = 5;
	CurrentCount = 0;
}

#if WITH_EDITOR
void UDWTask_GenerateVoxel::OnGenerate()
{
	Super::OnGenerate();
}

void UDWTask_GenerateVoxel::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWTask_GenerateVoxel::OnInitialize()
{
	Super::OnInitialize();
}

void UDWTask_GenerateVoxel::OnRestore()
{
	Super::OnRestore();

	CurrentCount = 0;
}

void UDWTask_GenerateVoxel::OnEnter()
{
	Super::OnEnter();
}

void UDWTask_GenerateVoxel::OnRefresh()
{
	Super::OnRefresh();
}

void UDWTask_GenerateVoxel::OnGuide()
{
	Super::OnGuide();
}

void UDWTask_GenerateVoxel::OnExecute()
{
	Super::OnExecute();
	
	UEventModuleStatics::SubscribeEvent<UEventHandle_VoxelGenerated>(this, GET_FUNCTION_NAME_THISCLASS(OnVoxelGenerated));
}

void UDWTask_GenerateVoxel::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelGenerated>(this, GET_FUNCTION_NAME_THISCLASS(OnVoxelGenerated));
}

void UDWTask_GenerateVoxel::OnLeave()
{
	Super::OnLeave();

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelGenerated>(this, GET_FUNCTION_NAME_CHECKED(UDWTask_GenerateVoxel, OnVoxelGenerated));
}

void UDWTask_GenerateVoxel::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if(Ar.ArIsSaveGame)
	{
		if(Ar.IsLoading())
		{
			Ar << CurrentCount;
		}
		else if(Ar.IsSaving())
		{
			Ar << CurrentCount;
		}
	}
}

void UDWTask_GenerateVoxel::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UDWTask_GenerateVoxel::ToData()
{
	return Super::ToData();
}

float UDWTask_GenerateVoxel::CheckTaskProgress_Implementation(FString& OutInfo) const
{
	OutInfo = FString::Printf(TEXT("%d/%d"), CurrentCount, TargetCount);
	return (float)CurrentCount / TargetCount;
}

void UDWTask_GenerateVoxel::OnVoxelGenerated_Implementation(UObject* InSender, UEventHandle_VoxelGenerated* InEventHandle)
{
	if(InEventHandle->VoxelAgent == UCommonStatics::GetPlayerPawn() && (!TargetVoxelID.IsValid() || InEventHandle->VoxelItem.ID == TargetVoxelID))
	{
		CurrentCount++;
		Restate();
		if(CurrentCount >= TargetCount)
		{
			Complete(ETaskExecuteResult::Succeed);
		}
	}
}
