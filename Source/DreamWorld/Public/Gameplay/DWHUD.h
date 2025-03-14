// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/WHHUD.h"
#include "DWHUD.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API ADWHUD : public AWHHUD
{
	GENERATED_BODY()
	
	//////////////////////////////////////////////////////////////////////////
	/// WHActor
public:
	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation() override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnTermination_Implementation() override;
};
