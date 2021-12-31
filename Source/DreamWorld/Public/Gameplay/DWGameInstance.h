// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Gameplay/WHGameInstance.h"
#include "DWGameInstance.generated.h"

/**
 * 游戏实例基类
 */
UCLASS()
class DREAMWORLD_API UDWGameInstance : public UWHGameInstance
{
	GENERATED_BODY()

public:
	UDWGameInstance();

public:
	virtual void Init() override;

	virtual void Shutdown() override;

public:
	void InitializeData();

	//////////////////////////////////////////////////////////////////////////
	// GeneralData
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GeneralData")
	class UGeneralSaveGame* GeneralSaveGame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GeneralData")
	int32 UserIndex;

public:
	UFUNCTION(BlueprintCallable)
	void CreateGeneralData(FGeneralSaveData InGeneralData, bool bSaveData = true);

	UFUNCTION(BlueprintCallable)
	void ResetGeneralData();

	UFUNCTION(BlueprintCallable)
	void SaveGeneralData(bool bRefresh = true);

	UFUNCTION(BlueprintCallable)
	UGeneralSaveGame* LoadGeneralData();
	
	UFUNCTION(BlueprintPure)
	FGeneralSaveData GetGeneralData() const;

	UFUNCTION(BlueprintPure)
	int32 GetUserIndex() const { return UserIndex; }

	UFUNCTION(BlueprintCallable)
	void SetUserIndex(int32 InUserIndex) { UserIndex = InUserIndex; }

	//////////////////////////////////////////////////////////////////////////
	// ArchiveData
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArchiveData")
	class UArchiveSaveGame* ArchiveSaveGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ArchiveData")
	FPlayerBasicSaveData PlayerBasicData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ArchiveData")
	FWorldBasicSaveData WorldBasicData;
	
public:
	UFUNCTION(BlueprintCallable)
	void CreateArchiveData(FArchiveSaveData InArchiveData);

	UFUNCTION(BlueprintCallable)
	void RemoveArchiveData(FName InArchiveID);
	
	UFUNCTION(BlueprintCallable)
	void SaveArchiveData(FName InArchiveID, bool bRefresh = true);

	UFUNCTION(BlueprintCallable)
	bool IsExistArchiveData(FName InArchiveID);
		
	UFUNCTION(BlueprintCallable)
	class UArchiveSaveGame* LoadArchiveData(FName InArchiveID = NAME_None);

public:
	UFUNCTION(BlueprintPure)
	FName GetCurrentArchiveID() const;

	UFUNCTION(BlueprintPure)
	TMap<FName, FArchiveSaveData>& GetArchiveDatas() const;

	FPlayerBasicSaveData GetPlayerBasicData() const { return PlayerBasicData; }

	FWorldBasicSaveData GetWorldBasicData() const { return WorldBasicData; }
};
