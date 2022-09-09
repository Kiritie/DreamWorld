// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterModule.h"
	
MODULE_INSTANCE_IMPLEMENTATION(ADWCharacterModule)

// ParamSets default values
ADWCharacterModule::ADWCharacterModule()
{
	PlayerBasicData = FDWPlayerBasicSaveData();
}

#if WITH_EDITOR
void ADWCharacterModule::OnGenerate_Implementation()
{
	Super::OnGenerate_Implementation();
}

void ADWCharacterModule::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
#endif

void ADWCharacterModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWCharacterModule::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
}

void ADWCharacterModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWCharacterModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWCharacterModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}
