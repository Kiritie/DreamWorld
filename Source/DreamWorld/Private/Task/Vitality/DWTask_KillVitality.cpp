// Fill out your copyright notice in the Description page of Project Settings.

#include "Task/Vitality/DWTask_KillVitality.h"

#include "TimerManager.h"
#include "Ability/Vitality/AbilityVitalityInterface.h"
#include "Asset/Primary/PrimaryEntityInterface.h"
#include "Event/EventModuleStatics.h"
#include "Common/CommonStatics.h"
#include "Event/Handle/Ability/EventHandle_VitalityDead.h"

UDWTask_KillVitality::UDWTask_KillVitality()
{
	TaskDisplayName = FText::FromString(TEXT("击杀目标"));
	TaskDescription = FText::FromString(TEXT("击杀XX个XX"));

	TargetVitalityID = FPrimaryAssetId();
	TargetCount = 5;
	CurrentCount = 0;
}

#if WITH_EDITOR
void UDWTask_KillVitality::OnGenerate()
{
	Super::OnGenerate();
}

void UDWTask_KillVitality::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWTask_KillVitality::OnInitialize()
{
	Super::OnInitialize();
}

void UDWTask_KillVitality::OnRestore()
{
	Super::OnRestore();

	CurrentCount = 0;
}

void UDWTask_KillVitality::OnEnter()
{
	Super::OnEnter();
}

void UDWTask_KillVitality::OnRefresh()
{
	Super::OnRefresh();
}

void UDWTask_KillVitality::OnGuide()
{
	Super::OnGuide();
}

void UDWTask_KillVitality::OnExecute()
{
	Super::OnExecute();
	
	UEventModuleStatics::SubscribeEvent<UEventHandle_VitalityDead>(this, GET_FUNCTION_NAME_THISCLASS(OnVitalityDead));
}

void UDWTask_KillVitality::OnComplete(ETaskExecuteResult InTaskExecuteResult)
{
	Super::OnComplete(InTaskExecuteResult);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VitalityDead>(this, GET_FUNCTION_NAME_THISCLASS(OnVitalityDead));
}

void UDWTask_KillVitality::OnLeave()
{
	Super::OnLeave();

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VitalityDead>(this, GET_FUNCTION_NAME_THISCLASS(OnVitalityDead));
}

void UDWTask_KillVitality::Serialize(FArchive& Ar)
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

void UDWTask_KillVitality::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* UDWTask_KillVitality::ToData()
{
	return Super::ToData();
}

float UDWTask_KillVitality::CheckTaskProgress_Implementation(FString& OutInfo) const
{
	OutInfo = FString::Printf(TEXT("%d/%d"), CurrentCount, TargetCount);
	return (float)CurrentCount / TargetCount;
}

void UDWTask_KillVitality::OnVitalityDead_Implementation(UObject* InSender, UEventHandle_VitalityDead* InEventHandle)
{
	if(InEventHandle->Killer == UCommonStatics::GetPlayerPawn() && (!TargetVitalityID.IsValid() || IPrimaryEntityInterface::Execute_GetAssetID(InEventHandle->Vitality.GetObject()) == TargetVitalityID))
	{
		CurrentCount++;
		Restate();
		if(CurrentCount >= TargetCount)
		{
			Complete(ETaskExecuteResult::Succeed);
		}
	}
}
