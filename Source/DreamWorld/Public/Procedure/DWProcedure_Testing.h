// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Procedure/Base/DWProcedureBase.h"
#include "DWProcedure_Testing.generated.h"

class ADWPlayerSpawner;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWProcedure_Testing : public UDWProcedureBase
{
	GENERATED_BODY()
	
public:
	UDWProcedure_Testing();

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

protected:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<ADWPlayerSpawner> PlayerSpawner;
};
