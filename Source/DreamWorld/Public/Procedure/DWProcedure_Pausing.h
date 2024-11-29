// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Procedure/Base/DWProcedureBase.h"
#include "DWProcedure_Pausing.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWProcedure_Pausing : public UDWProcedureBase
{
	GENERATED_BODY()
	
public:
	UDWProcedure_Pausing();

	//////////////////////////////////////////////////////////////////////////
	/// Procedure
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnUnGenerate() override;
#endif

public:
	virtual void OnInitialize() override;
	
	virtual void OnEnter(UProcedureBase* InLastProcedure) override;
	
	virtual void OnRefresh() override;

	virtual void OnGuide() override;
	
	virtual void OnLeave(UProcedureBase* InNextProcedure) override;
};
