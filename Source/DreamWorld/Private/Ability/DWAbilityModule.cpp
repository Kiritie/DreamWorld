// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DWAbilityModule.h"

#include "Ability/AbilityModuleBPLibrary.h"
#include "Global/DWGlobalTypes.h"

IMPLEMENTATION_MODULE(ADWAbilityModule)

// ParamSets default values
ADWAbilityModule::ADWAbilityModule()
{
	
}

ADWAbilityModule::~ADWAbilityModule()
{
	TERMINATION_MODULE(ADWAbilityModule)
}

#if WITH_EDITOR
void ADWAbilityModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWAbilityModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWAbilityModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
	
	UAbilityModuleBPLibrary::AddCustomInteractAction((int32)EDWInteractAction::Feed, TEXT("/Script/DreamWorld.EDWInteractAction"));
	UAbilityModuleBPLibrary::AddCustomInteractAction((int32)EDWInteractAction::Ride, TEXT("/Script/DreamWorld.EDWInteractAction"));
	UAbilityModuleBPLibrary::AddCustomInteractAction((int32)EDWInteractAction::UnRide, TEXT("/Script/DreamWorld.EDWInteractAction"));
}

void ADWAbilityModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWAbilityModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWAbilityModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWAbilityModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}
