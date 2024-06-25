// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DWAbilityModule.h"

#include "Ability/AbilityModuleStatics.h"
#include "Common/DWCommonTypes.h"
#include "Scene/SceneModuleStatics.h"

IMPLEMENTATION_MODULE(UDWAbilityModule)

// ParamSets default values
UDWAbilityModule::UDWAbilityModule()
{
	
}

UDWAbilityModule::~UDWAbilityModule()
{
	TERMINATION_MODULE(UDWAbilityModule)
}

#if WITH_EDITOR
void UDWAbilityModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWAbilityModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWAbilityModule)
}
#endif

void UDWAbilityModule::OnInitialize()
{
	Super::OnInitialize();
	
	USceneModuleStatics::AddTraceMapping(FName("PickUp"), (ECollisionChannel)EDWGameTraceChannel::Chunk);
}

void UDWAbilityModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWAbilityModule::OnRefresh(float DeltaSeconds, bool bInEditor)
{
	Super::OnRefresh(DeltaSeconds, bInEditor);
}

void UDWAbilityModule::OnPause()
{
	Super::OnPause();
}

void UDWAbilityModule::OnUnPause()
{
	Super::OnUnPause();
}
