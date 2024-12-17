// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Base/DWTaskBase.h"

#include "TimerManager.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Context/WidgetContextBox.h"

UDWTaskBase::UDWTaskBase()
{
	TaskDisplayName = FText::FromString(TEXT("根任务"));
	
	Level = 0;
	Prizes = TArray<FAbilityItem>();
}

#if WITH_EDITOR
void UDWTaskBase::OnGenerate()
{
	Super::OnGenerate();
}

void UDWTaskBase::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWTaskBase::OnInitialize()
{
	Super::OnInitialize();
}

void UDWTaskBase::OnRestore()
{
	Super::OnRestore();
}

void UDWTaskBase::OnEnter()
{
	Super::OnEnter();
}

void UDWTaskBase::OnRefresh()
{
	Super::OnRefresh();
}

void UDWTaskBase::OnGuide()
{
	Super::OnGuide();
}

void UDWTaskBase::OnExecute()
{
	Super::OnExecute();
}

void UDWTaskBase::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);

	if(InTaskExecuteResult != ETaskExecuteResult::Skipped)
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("任务%s: %s"), TaskExecuteResult != ETaskExecuteResult::Skipped ? TEXT("完成") : TEXT("失败"), *TaskDisplayName.ToString()));
		}
	}
}

void UDWTaskBase::OnLeave()
{
	Super::OnLeave();

	if(TaskExecuteResult == ETaskExecuteResult::Succeed)
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

bool UDWTaskBase::CheckTaskCondition_Implementation(FString& OutInfo) const
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
