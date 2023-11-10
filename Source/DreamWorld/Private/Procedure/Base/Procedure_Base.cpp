// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Base/Procedure_Base.h"

#include "Audio/AudioModuleStatics.h"
#include "Camera/CameraModuleStatics.h"

FSingleSoundHandle UProcedure_Base::BGMHandle = FSingleSoundHandle();

UProcedure_Base::UProcedure_Base()
{
	ProcedureDisplayName = FText::FromString(TEXT("根流程"));
	BGMSound = nullptr;
	CameraRotationSpeed = 0.f;
}

#if WITH_EDITOR
void UProcedure_Base::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Base::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Base::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Base::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	if(BGMHandle.IsValid() && UAudioModuleStatics::GetSingleSoundInfo(BGMHandle).Sound != BGMSound)
	{
		UAudioModuleStatics::StopSingleSound(BGMHandle);
		BGMHandle = -1;
	}
	if(!BGMHandle.IsValid() && BGMSound)
	{
		BGMHandle = UAudioModuleStatics::PlaySingleSound2D(BGMSound);
	}
}

void UProcedure_Base::OnRefresh()
{
	Super::OnRefresh();

	if(CameraRotationSpeed != 0.f)
	{
		UCameraModuleStatics::AddCameraRotationInput(CameraRotationSpeed, 0.f);
	}
}

void UProcedure_Base::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Base::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
