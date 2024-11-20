// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Voxel/Task_DestroyVoxel.h"

#include "TimerManager.h"
#include "Event/EventModuleStatics.h"
#include "Common/CommonStatics.h"
#include "Event/Handle/Voxel/EventHandle_VoxelDestroyed.h"

UTask_DestroyVoxel::UTask_DestroyVoxel()
{
	TaskDisplayName = FText::FromString(TEXT("破坏"));
	TaskDescription = FText::FromString(TEXT("破坏XX个方块"));

	TargetCount = 5;
	CurrentCount = 0;
}

#if WITH_EDITOR
void UTask_DestroyVoxel::OnGenerate()
{
	Super::OnGenerate();
}

void UTask_DestroyVoxel::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UTask_DestroyVoxel::OnInitialize()
{
	Super::OnInitialize();
}

void UTask_DestroyVoxel::OnRestore()
{
	Super::OnRestore();

	CurrentCount = 0;
}

void UTask_DestroyVoxel::OnEnter(UTaskBase* InLastTask)
{
	Super::OnEnter(InLastTask);
}

void UTask_DestroyVoxel::OnRefresh()
{
	Super::OnRefresh();
}

void UTask_DestroyVoxel::OnGuide()
{
	Super::OnGuide();
}

void UTask_DestroyVoxel::OnExecute()
{
	Super::OnExecute();
	
	UEventModuleStatics::SubscribeEvent<UEventHandle_VoxelDestroyed>(this, GET_FUNCTION_NAME_THISCLASS(OnVoxelDestroyed));
}

void UTask_DestroyVoxel::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelDestroyed>(this, GET_FUNCTION_NAME_THISCLASS(OnVoxelDestroyed));
}

void UTask_DestroyVoxel::OnLeave()
{
	Super::OnLeave();

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelDestroyed>(this, GET_FUNCTION_NAME_THISCLASS(OnVoxelDestroyed));
}

void UTask_DestroyVoxel::Serialize(FArchive& Ar)
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

void UTask_DestroyVoxel::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UTask_DestroyVoxel::ToData()
{
	return Super::ToData();
}

float UTask_DestroyVoxel::CheckTaskProgress_Implementation(FString& OutInfo) const
{
	OutInfo = FString::Printf(TEXT("%d/%d"), CurrentCount, TargetCount);
	return (float)CurrentCount / TargetCount;
}

void UTask_DestroyVoxel::OnVoxelDestroyed_Implementation(UObject* InSender, UEventHandle_VoxelDestroyed* InEventHandle)
{
	CurrentCount++;
	RefreshState();
	if(CurrentCount >= TargetCount)
	{
		Complete(ETaskExecuteResult::Succeed);
	}
}
