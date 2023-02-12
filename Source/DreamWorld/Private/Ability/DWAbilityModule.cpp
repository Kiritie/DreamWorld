// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DWAbilityModule.h"
	
IMPLEMENTATION_MODULE(ADWAbilityModule)

// ParamSets default values
ADWAbilityModule::ADWAbilityModule()
{
	
}

#if WITH_EDITOR
void ADWAbilityModule::OnGenerate_Implementation()
{
	Super::OnGenerate_Implementation();
}

void ADWAbilityModule::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
#endif

void ADWAbilityModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
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
