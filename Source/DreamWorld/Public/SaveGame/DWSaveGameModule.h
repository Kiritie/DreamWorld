// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/SaveGameModule.h"

#include "DWSaveGameModule.generated.h"

UCLASS()
class DREAMWORLD_API ADWSaveGameModule : public ASaveGameModule
{
	GENERATED_BODY()
	
public:	
	// ParamSets default values for this actor's properties
	ADWSaveGameModule();
	
	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate_Implementation() override;

	virtual void OnDestroy_Implementation() override;
#endif

	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation() override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;

	//////////////////////////////////////////////////////////////////////////
	// ArchiveData
protected:
	UPROPERTY(EditAnywhere, Category = "ArchiveData")
	FArchiveBasicSaveData ArchiveBasicData;

public:
	UFUNCTION(BlueprintPure)
	int32 GetNewValidArchiveID() const;

	UFUNCTION(BlueprintPure)
	FPlayerSaveData GetDefaultPlayerData() const;

	UFUNCTION(BlueprintPure)
	FDWWorldSaveData GetDefaultWorldData() const;
};
