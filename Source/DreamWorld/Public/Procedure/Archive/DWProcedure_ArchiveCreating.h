// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Procedure/Base/DWProcedureBase.h"
#include "Common/DWCommonTypes.h"
#include "DWProcedure_ArchiveCreating.generated.h"

class UFiniteStateBase;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWProcedure_ArchiveCreating : public UDWProcedureBase
{
	GENERATED_BODY()
	
public:
	UDWProcedure_ArchiveCreating();

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
	void CreatePlayer(UPARAM(ref) FDWPlayerSaveData& InPlayerSaveData, EPhase InPhase);

	UFUNCTION(BlueprintCallable)
	void CreateWorld(UPARAM(ref) FDWWorldSaveData& InWorldSaveData, EPhase InPhase);

	UFUNCTION(BlueprintCallable)
	void CreateArchive(UPARAM(ref) FDWArchiveSaveData& InArchiveSaveData);
	
protected:
	UPROPERTY(EditAnywhere, Category = "SaveData")
	FDWPlayerSaveData PlayerSaveData;

	UPROPERTY(EditAnywhere, Category = "SaveData")
	FDWWorldSaveData WorldSaveData;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinCameraDistance;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxCameraDistance;

private:
	float LocalMinCameraDistance;
	float LocalMaxCameraDistance;

public:
	UFUNCTION(BlueprintPure)
	FDWPlayerSaveData& GetPlayerData() const;

	UFUNCTION(BlueprintPure)
	FDWWorldSaveData& GetWorldData() const;
};
