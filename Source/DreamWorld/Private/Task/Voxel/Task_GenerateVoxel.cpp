// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Voxel/Task_GenerateVoxel.h"

#include "TimerManager.h"
#include "Event/EventModuleStatics.h"
#include "Common/CommonStatics.h"
#include "Event/Handle/Voxel/EventHandle_VoxelGenerated.h"

UTask_GenerateVoxel::UTask_GenerateVoxel()
{
	TaskDisplayName = FText::FromString(TEXT("建造"));
	TaskDescription = FText::FromString(TEXT("建造5个方块"));

	MaxCount = 5;
	CurrentCount = 0;
}

#if WITH_EDITOR
void UTask_GenerateVoxel::OnGenerate()
{
	Super::OnGenerate();
}

void UTask_GenerateVoxel::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UTask_GenerateVoxel::OnInitialize()
{
	Super::OnInitialize();
}

void UTask_GenerateVoxel::OnRestore()
{
	Super::OnRestore();

	CurrentCount = 0;
}

void UTask_GenerateVoxel::OnEnter(UTaskBase* InLastTask)
{
	Super::OnEnter(InLastTask);
}

void UTask_GenerateVoxel::OnRefresh()
{
	Super::OnRefresh();
}

void UTask_GenerateVoxel::OnGuide()
{
	Super::OnGuide();
}

void UTask_GenerateVoxel::OnExecute()
{
	Super::OnExecute();
	
	UEventModuleStatics::SubscribeEvent<UEventHandle_VoxelGenerated>(this, FName("OnVoxelGenerated"));
}

void UTask_GenerateVoxel::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelGenerated>(this, FName("OnVoxelGenerated"));
}

void UTask_GenerateVoxel::OnLeave()
{
	Super::OnLeave();

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelGenerated>(this, FName("OnVoxelGenerated"));
}

void UTask_GenerateVoxel::Serialize(FArchive& Ar)
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

void UTask_GenerateVoxel::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UTask_GenerateVoxel::ToData()
{
	return Super::ToData();
}

float UTask_GenerateVoxel::CheckTaskProgress_Implementation(FString& OutInfo) const
{
	OutInfo = FString::Printf(TEXT("%d/%d"), CurrentCount, MaxCount);
	return (float)CurrentCount / MaxCount;
}

void UTask_GenerateVoxel::OnVoxelGenerated_Implementation(UObject* InSender, UEventHandle_VoxelGenerated* InEventHandle)
{
	CurrentCount++;
	if(CurrentCount >= MaxCount)
	{
		Complete(ETaskExecuteResult::Succeed);
	}
}
