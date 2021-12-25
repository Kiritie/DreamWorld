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
	void Initialize();

	//////////////////////////////////////////////////////////////////////////
	// GeneralData
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USaveGameGeneral* GeneralDataSave;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 UserIndex;

public:
	UFUNCTION(BlueprintCallable)
	void ResetGeneralData();

	UFUNCTION(BlueprintCallable)
	void SaveGeneralData(bool bRefresh = true);

	UFUNCTION(BlueprintCallable)
	USaveGameGeneral* LoadGeneralData();
	
	UFUNCTION(BlueprintCallable)
	void CreateGeneralData(FGeneralSaveData InGeneralData, bool bSaveData = true);

	UFUNCTION(BlueprintCallable)
	void DeleteGeneralData();

	UFUNCTION(BlueprintPure)
	FGeneralSaveData GetGeneralData() const;

	UFUNCTION(BlueprintPure)
	int32 GetUserIndex() const { return UserIndex; }

	UFUNCTION(BlueprintCallable)
	void SetUserIndex(int32 InUserIndex) { UserIndex = InUserIndex; }

	//////////////////////////////////////////////////////////////////////////
	// ArchiveData
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FString, class USaveGameArchive*> ArchiveDataSaves;

public:
	UFUNCTION(BlueprintCallable)
	bool IsExistArchiveData(const FString& InArchiveName);
	
	UFUNCTION(BlueprintCallable)
	void SaveArchiveData(const FString& InArchiveName, bool bRefresh = true);
		
	UFUNCTION(BlueprintCallable)
	class USaveGameArchive* LoadArchiveData(FString InArchiveName = TEXT(""));
	
	UFUNCTION(BlueprintCallable)
	void UnloadArchiveData(const FString& InArchiveName, bool bSaveData = true);

	UFUNCTION(BlueprintCallable)
	void CreateArchiveData(FArchiveSaveData InArchiveData, bool bSaveData = true);

	UFUNCTION(BlueprintCallable)
	void RemoveArchiveData(const FString& InArchiveName);

	UFUNCTION(BlueprintPure)
	FString GetCurrentArchiveName() const;

	UFUNCTION(BlueprintPure)
	TMap<FString, FArchiveSaveData> GetArchiveDatas() const;
};
