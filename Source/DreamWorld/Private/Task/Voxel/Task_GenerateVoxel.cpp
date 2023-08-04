// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Voxel/Task_GenerateVoxel.h"

#include "TimerManager.h"
#include "Event/EventModuleBPLibrary.h"
#include "Event/Handle/Voxel/EventHandle_GenerateVoxel.h"
#include "Gameplay/DWGameState.h"
#include "Global/GlobalBPLibrary.h"
#include "Widget/WidgetMainMenu.h"
#include "Widget/WidgetModuleBPLibrary.h"

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

	UEventModuleBPLibrary::SubscribeEvent<UEventHandle_GenerateVoxel>(this, FName("OnGenerateVoxel"));
}

void UTask_GenerateVoxel::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);

	UEventModuleBPLibrary::UnsubscribeEvent<UEventHandle_GenerateVoxel>(this, FName("OnGenerateVoxel"));
}

void UTask_GenerateVoxel::OnLeave()
{
	Super::OnLeave();
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

FSaveData* UTask_GenerateVoxel::ToData(bool bRefresh)
{
	return Super::ToData(bRefresh);
}

float UTask_GenerateVoxel::CheckTaskProgress_Implementation(FString& OutInfo) const
{
	OutInfo = FString::Printf(TEXT("%d/%d"), CurrentCount, MaxCount);
	return (float)CurrentCount / MaxCount;
}

void UTask_GenerateVoxel::OnGenerateVoxel_Implementation(UObject* InSender, UEventHandle_GenerateVoxel* InEventHandle)
{
	CurrentCount++;
	if(CurrentCount >= MaxCount)
	{
		Complete(ETaskExecuteResult::Succeed);
	}
}
