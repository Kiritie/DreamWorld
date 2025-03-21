// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Main/DWMainModule.h"

#include "Common/CommonStatics.h"

IMPLEMENTATION_MAIN_MODULE(ADWMainModule)

ADWMainModule::ADWMainModule()
{
}

ADWMainModule::~ADWMainModule()
{
	TERMINATION_MAIN_MODULE(ADWMainModule)
}

void ADWMainModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWMainModule::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
}
