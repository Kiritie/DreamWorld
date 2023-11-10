// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DWAbilityModule.h"

#include "Ability/AbilityModuleStatics.h"
#include "Common/DWCommonTypes.h"

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
}

void UDWAbilityModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);

	if(PHASEC(InPhase, EPhase::Primary))
	{
		UAssetModuleStatics::AddEnumMapping(TEXT("/Script/WHFramework.EInteractAction"), TEXT("/Script/WHFramework.EDWInteractAction"));
	}
}

void UDWAbilityModule::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWAbilityModule::OnPause()
{
	Super::OnPause();
}

void UDWAbilityModule::OnUnPause()
{
	Super::OnUnPause();
}

ECollisionChannel UDWAbilityModule::GetPickUpTraceChannel() const
{
	return (ECollisionChannel)EDWGameTraceChannel::Chunk;
}
