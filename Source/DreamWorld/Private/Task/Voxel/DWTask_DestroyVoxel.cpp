// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Voxel/DWTask_DestroyVoxel.h"

#include "TimerManager.h"
#include "Event/EventModuleStatics.h"
#include "Common/CommonStatics.h"
#include "Event/Handle/Voxel/EventHandle_VoxelDestroyed.h"
#include "Voxel/Agent/VoxelAgentInterface.h"

UDWTask_DestroyVoxel::UDWTask_DestroyVoxel()
{
	TaskDisplayName = FText::FromString(TEXT("破坏"));
	TaskDescription = FText::FromString(TEXT("破坏XX个方块"));

	TargetVoxelID = FPrimaryAssetId();
	TargetCount = 5;
	CurrentCount = 0;
}

#if WITH_EDITOR
void UDWTask_DestroyVoxel::OnGenerate()
{
	Super::OnGenerate();
}

void UDWTask_DestroyVoxel::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWTask_DestroyVoxel::OnInitialize()
{
	Super::OnInitialize();
}

void UDWTask_DestroyVoxel::OnRestore()
{
	Super::OnRestore();

	CurrentCount = 0;
}

void UDWTask_DestroyVoxel::OnEnter(UTaskBase* InLastTask)
{
	Super::OnEnter(InLastTask);
}

void UDWTask_DestroyVoxel::OnRefresh()
{
	Super::OnRefresh();
}

void UDWTask_DestroyVoxel::OnGuide()
{
	Super::OnGuide();
}

void UDWTask_DestroyVoxel::OnExecute()
{
	Super::OnExecute();
	
	UEventModuleStatics::SubscribeEvent<UEventHandle_VoxelDestroyed>(this, GET_FUNCTION_NAME_THISCLASS(OnVoxelDestroyed));
}

void UDWTask_DestroyVoxel::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelDestroyed>(this, GET_FUNCTION_NAME_THISCLASS(OnVoxelDestroyed));
}

void UDWTask_DestroyVoxel::OnLeave()
{
	Super::OnLeave();

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelDestroyed>(this, GET_FUNCTION_NAME_THISCLASS(OnVoxelDestroyed));
}

void UDWTask_DestroyVoxel::Serialize(FArchive& Ar)
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

void UDWTask_DestroyVoxel::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UDWTask_DestroyVoxel::ToData()
{
	return Super::ToData();
}

float UDWTask_DestroyVoxel::CheckTaskProgress_Implementation(FString& OutInfo) const
{
	OutInfo = FString::Printf(TEXT("%d/%d"), CurrentCount, TargetCount);
	return (float)CurrentCount / TargetCount;
}

void UDWTask_DestroyVoxel::OnVoxelDestroyed_Implementation(UObject* InSender, UEventHandle_VoxelDestroyed* InEventHandle)
{
	if(InEventHandle->VoxelAgent == UCommonStatics::GetPlayerPawn() && (!TargetVoxelID.IsValid() || InEventHandle->VoxelItem.ID == TargetVoxelID))
	{
		CurrentCount++;
		RefreshState();
		if(CurrentCount >= TargetCount)
		{
			Complete(ETaskExecuteResult::Succeed);
		}
	}
}
