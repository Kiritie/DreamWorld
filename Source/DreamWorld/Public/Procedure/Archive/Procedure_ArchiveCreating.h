// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Procedure/Base/Procedure_Base.h"
#include "DWTypes.h"
#include "Procedure_ArchiveCreating.generated.h"

class UFiniteStateBase;
class UEventHandle_ChangeVoxelWorldState;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UProcedure_ArchiveCreating : public UProcedure_Base
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

protected:
	UFUNCTION()
	void OnPlayerChanged(APawn* InPlayerPawn);

public:
	UFUNCTION(BlueprintCallable)
	void CreatePlayer(UPARAM(ref) FDWPlayerSaveData& InPlayerSaveData, EPhase InPhase = EPhase::Primary);

	UFUNCTION(BlueprintCallable)
	void CreateWorld(UPARAM(ref) FDWVoxelWorldSaveData& InWorldSaveData, EPhase InPhase = EPhase::Primary);

	UFUNCTION(BlueprintCallable)
	void CreateArchive(UPARAM(ref) FDWArchiveSaveData& InArchiveSaveData);
};
