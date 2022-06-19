// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Procedure/Base/ProcedureBase.h"
#include "Procedure_ArchiveCreating.generated.h"

class UEventHandle_ChangeVoxelWorldState;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UProcedure_ArchiveCreating : public UProcedureBase
{
	GENERATED_BODY()
	
public:
	UProcedure_ArchiveCreating();

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

public:
	UFUNCTION(BlueprintCallable)
	void CreateArchive(FDWArchiveSaveData InArchiveSaveData);

protected:
	UFUNCTION()
	void OnChangeVoxelWorldState(UObject* InSender, UEventHandle_ChangeVoxelWorldState* InEventHandle);
};
