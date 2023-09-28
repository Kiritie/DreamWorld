// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DWAbilityModule.h"

#include "Ability/AbilityModuleBPLibrary.h"
#include "Common/DWCommonTypes.h"

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
}

void ADWAbilityModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);

	if(PHASEC(InPhase, EPhase::Primary))
	{
		UAssetModuleBPLibrary::AddEnumMapping(TEXT("/Script/WHFramework.EInteractAction"), TEXT("/Script/WHFramework.EDWInteractAction"));
	}
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

ECollisionChannel ADWAbilityModule::GetPickUpTraceChannel() const
{
	return (ECollisionChannel)EDWGameTraceChannel::Chunk;
}
