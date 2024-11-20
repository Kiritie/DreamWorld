// Fill out your copyright notice in the Description page of Project Settings.

#include "Task/Role/Task_LevelUp.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"

UTask_LevelUp::UTask_LevelUp()
{
	TaskDisplayName = FText::FromString(TEXT("等级提升"));
	TaskDescription = FText::FromString(TEXT("提升角色等级到XX级"));

	TargetLevel = 0;
	CurrentLevel = 0;
}

#if WITH_EDITOR
void UTask_LevelUp::OnGenerate()
{
	Super::OnGenerate();
}

void UTask_LevelUp::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UTask_LevelUp::OnInitialize()
{
	Super::OnInitialize();
}

void UTask_LevelUp::OnRestore()
{
	Super::OnRestore();
}

void UTask_LevelUp::OnEnter(UTaskBase* InLastTask)
{
	Super::OnEnter(InLastTask);
}

void UTask_LevelUp::OnRefresh()
{
	Super::OnRefresh();

	const ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	if(PlayerCharacter && CurrentLevel != PlayerCharacter->GetLevelA())
	{
		CurrentLevel = PlayerCharacter->GetLevelA();
		RefreshState();
		if(CurrentLevel >= TargetLevel)
		{
			Complete(ETaskExecuteResult::Succeed);
		}
	}
}

void UTask_LevelUp::OnGuide()
{
	Super::OnGuide();
}

void UTask_LevelUp::OnExecute()
{
	Super::OnExecute();
}

void UTask_LevelUp::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);
}

void UTask_LevelUp::OnLeave()
{
	Super::OnLeave();
}

void UTask_LevelUp::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if(Ar.ArIsSaveGame)
	{
		if(Ar.IsLoading())
		{
			Ar << CurrentLevel;
		}
		else if(Ar.IsSaving())
		{
			Ar << CurrentLevel;
		}
	}
}

void UTask_LevelUp::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UTask_LevelUp::ToData()
{
	return Super::ToData();
}

float UTask_LevelUp::CheckTaskProgress_Implementation(FString& OutInfo) const
{
	const ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	if(PlayerCharacter)
	{
		OutInfo = FString::Printf(TEXT("%d/%d"), PlayerCharacter->GetLevelA(), TargetLevel);
		return (float)PlayerCharacter->GetLevelA() / TargetLevel;
	}
	return 0.f;
}
