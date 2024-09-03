// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/Procedure_Testing.h"

#include "Ability/Spawner/DWPlayerSpawner.h"
#include "Procedure/Procedure_Loading.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"

UProcedure_Testing::UProcedure_Testing()
{
	ProcedureName = FName("Testing");
	ProcedureDisplayName = FText::FromString(TEXT("测试"));
}

#if WITH_EDITOR
void UProcedure_Testing::OnGenerate()
{
	Super::OnGenerate();
}

void UProcedure_Testing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UProcedure_Testing::OnInitialize()
{
	Super::OnInitialize();
}

void UProcedure_Testing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	UWidgetGameHUD* GameHUD = UWidgetModuleStatics::CreateUserWidget<UWidgetGameHUD>();

	if(ADWPlayerSpawner* _PlayerSpawner = PlayerSpawner.LoadSynchronous())
	{
		_PlayerSpawner->Spawn();

		GameHUD->Init(_PlayerSpawner->GetAbilityActor());
	}

	GameHUD->Open();
}

void UProcedure_Testing::OnRefresh()
{
	Super::OnRefresh();
}

void UProcedure_Testing::OnGuide()
{
	Super::OnGuide();
}

void UProcedure_Testing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
