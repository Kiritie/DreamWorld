// Fill out your copyright notice in the Description page of Project Settings.

#include "Task/Role/DWTask_LevelUp.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"

UDWTask_LevelUp::UDWTask_LevelUp()
{
	TaskDisplayName = FText::FromString(TEXT("等级提升"));
	TaskDescription = FText::FromString(TEXT("提升角色等级到XX级"));

	TargetLevel = 0;
	CurrentLevel = 0;
}

#if WITH_EDITOR
void UDWTask_LevelUp::OnGenerate()
{
	Super::OnGenerate();
}

void UDWTask_LevelUp::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWTask_LevelUp::OnInitialize()
{
	Super::OnInitialize();
}

void UDWTask_LevelUp::OnRestore()
{
	Super::OnRestore();
}

void UDWTask_LevelUp::OnEnter(UTaskBase* InLastTask)
{
	Super::OnEnter(InLastTask);
}

void UDWTask_LevelUp::OnRefresh()
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

void UDWTask_LevelUp::OnGuide()
{
	Super::OnGuide();
}

void UDWTask_LevelUp::OnExecute()
{
	Super::OnExecute();
}

void UDWTask_LevelUp::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);
}

void UDWTask_LevelUp::OnLeave()
{
	Super::OnLeave();
}

void UDWTask_LevelUp::Serialize(FArchive& Ar)
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

void UDWTask_LevelUp::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UDWTask_LevelUp::ToData()
{
	return Super::ToData();
}

float UDWTask_LevelUp::CheckTaskProgress_Implementation(FString& OutInfo) const
{
	const ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	if(PlayerCharacter)
	{
		OutInfo = FString::Printf(TEXT("%d/%d"), PlayerCharacter->GetLevelA(), TargetLevel);
		return (float)PlayerCharacter->GetLevelA() / TargetLevel;
	}
	return 0.f;
}
