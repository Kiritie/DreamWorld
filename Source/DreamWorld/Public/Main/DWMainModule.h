// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Main/MainModule.h"

#include "DWMainModule.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API ADWMainModule : public AMainModule
{
	GENERATED_BODY()
		
	GENERATED_MAIN_MODULE(ADWMainModule)

public:
	ADWMainModule();

	~ADWMainModule();

public:
	virtual void OnInitialize_Implementation() override;
	
	virtual void OnPreparatory_Implementation(EPhase InPhase) override;
};
