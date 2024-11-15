// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Base/Task_Base.h"

#include "TimerManager.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Context/WidgetContextBox.h"

UTask_Base::UTask_Base()
{
	TaskDisplayName = FText::FromString(TEXT("根任务"));
	
	Level = 0;
	Prizes = TArray<FAbilityItem>();
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

	if(InTaskExecuteResult != ETaskExecuteResult::Skipped)
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("任务%s: %s"), TaskExecuteResult != ETaskExecuteResult::Skipped ? TEXT("完成") : TEXT("失败"), *TaskDisplayName.ToString()));
		}
	}

	if(InTaskExecuteResult == ETaskExecuteResult::Succeed)
	{
		const ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
		if(PlayerCharacter && Prizes.Num() > 0)
		{
			for(auto Iter : Prizes)
			{
				PlayerCharacter->GetInventory()->AddItemByRange(Iter);
			}
		}
	}
}

void UTask_Base::OnLeave()
{
	Super::OnLeave();
}

bool UTask_Base::CheckTaskCondition_Implementation(FString& OutInfo) const
{
	if(const ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>())
	{
		if(Level > 0 && PlayerCharacter->GetLevelA() < Level)
		{
			OutInfo = FString::Printf(TEXT("角色未达到[%d]级"), Level);
			return false;
		}
	}
	return true;
}
