// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Base/Task_Base.h"

#include "TimerManager.h"
#include "Character/Player/DWPlayerCharacter.h"

UTask_Base::UTask_Base()
{
	TaskDisplayName = FText::FromString(TEXT("根任务"));
	NeedLevel = 1;
}

#if WITH_EDITOR
void UTask_Base::OnGenerate()
{
	Super::OnGenerate();
}

void UTask_Base::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UTask_Base::OnInitialize()
{
	Super::OnInitialize();
}

void UTask_Base::OnRestore()
{
	Super::OnRestore();
}

void UTask_Base::OnEnter(UTaskBase* InLastTask)
{
	Super::OnEnter(InLastTask);
}

void UTask_Base::OnRefresh()
{
	Super::OnRefresh();
}

void UTask_Base::OnGuide()
{
	Super::OnGuide();
}

void UTask_Base::OnExecute()
{
	Super::OnExecute();
}

void UTask_Base::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);
}

void UTask_Base::OnLeave()
{
	Super::OnLeave();
}

bool UTask_Base::CheckTaskCondition_Implementation(FString& OutInfo) const
{
	if(const ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerPawn<ADWPlayerCharacter>())
	{
		if(PlayerCharacter->GetLevelV() < NeedLevel)
		{
			OutInfo = FString::Printf(TEXT("角色等级未达到[%d]级"), NeedLevel);
			return false;
		}
	}
	return true;
}
