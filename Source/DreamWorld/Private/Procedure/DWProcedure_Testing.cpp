// Fill out your copyright notice in the Description page of Project Settings.


#include "Procedure/DWProcedure_Testing.h"

#include "Ability/Spawner/DWPlayerSpawner.h"
#include "Procedure/DWProcedure_Loading.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"

UDWProcedure_Testing::UDWProcedure_Testing()
{
	ProcedureName = FName("Testing");
	ProcedureDisplayName = FText::FromString(TEXT("测试"));
}

#if WITH_EDITOR
void UDWProcedure_Testing::OnGenerate()
{
	Super::OnGenerate();
}

void UDWProcedure_Testing::OnUnGenerate()
{
	Super::OnUnGenerate();
}
#endif

void UDWProcedure_Testing::OnInitialize()
{
	Super::OnInitialize();
}

void UDWProcedure_Testing::OnEnter(UProcedureBase* InLastProcedure)
{
	Super::OnEnter(InLastProcedure);

	UWidgetGameHUD* GameHUD = UWidgetModuleStatics::CreateUserWidget<UWidgetGameHUD>();

	if(ADWPlayerSpawner* Spawner = PlayerSpawner.LoadSynchronous())
	{
		Spawner->Spawn();

		GameHUD->Init(Spawner->GetAbilityActor());
	}

	GameHUD->Open();
}

void UDWProcedure_Testing::OnRefresh()
{
	Super::OnRefresh();
}

void UDWProcedure_Testing::OnGuide()
{
	Super::OnGuide();
}

void UDWProcedure_Testing::OnLeave(UProcedureBase* InNextProcedure)
{
	Super::OnLeave(InNextProcedure);
}
