// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Base/DWProcedureBase.h"

#include "Audio/AudioModuleStatics.h"
#include "Camera/CameraModuleStatics.h"

FSingleSoundHandle UDWProcedureBase::BGMHandle = FSingleSoundHandle();

UDWProcedureBase::UDWProcedureBase()
{
	ProcedureDisplayName = FText::FromString(TEXT("根流程"));
	BGMSound = nullptr;
	CameraRotationSpeed = 0.f;
}

#if WITH_EDITOR
void UDWProcedureBase::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedureBase::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedureBase::OnInitialize()
{
	Super::OnInitialize();
}

void UDWProcedureBase::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	if(BGMSound)
	{
		if(BGMHandle.IsValid() && UAudioModuleStatics::GetSingleSoundInfo(BGMHandle).Sound != BGMSound)
		{
			UAudioModuleStatics::StopSingleSound(BGMHandle);
			BGMHandle = -1;
		}
		if(!BGMHandle.IsValid())
		{
			BGMHandle = UAudioModuleStatics::PlaySingleSound2D(BGMSound);
		}
	}
}

void UDWProcedureBase::OnRefresh()
{
	Super::OnRefresh();

	if(CameraRotationSpeed != 0.f)
	{
		UCameraModuleStatics::AddCameraRotationInput(CameraRotationSpeed, 0.f);
	}
}

void UDWProcedureBase::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedureBase::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
