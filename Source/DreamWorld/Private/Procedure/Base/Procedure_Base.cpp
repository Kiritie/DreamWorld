// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Base/Procedure_Base.h"

#include "Audio/AudioModuleBPLibrary.h"
#include "Camera/CameraModuleBPLibrary.h"

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
	if(BGMSound)
	{
		BGMHandle = UAudioModuleBPLibrary::PlaySingleSound2D(BGMSound);
	}
	Super::OnEnter(InLastProcedure);
}

void UProcedure_Base::OnRefresh()
{
	Super::OnRefresh();

	if(CameraRotationSpeed != 0.f)
	{
		UCameraModuleBPLibrary::AddCameraRotationInput(CameraRotationSpeed, 0.f);
	}
}

void UProcedure_Base::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Base::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);

	if(BGMHandle.IsValid())
	{
		UAudioModuleBPLibrary::StopSingleSound(BGMHandle);
	}
}
