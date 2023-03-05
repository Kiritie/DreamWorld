// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Procedure/Base/Procedure_Base.h"
#include "Procedure_Overed.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UProcedure_Overed : public UProcedure_Base
{
	GENERATED_BODY()
	
public:
	UProcedure_Overed();

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
