// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Common/DWCommonTypes.h"
#include "Gameplay/WHGameMode.h"
#include "SaveGame/Base/SaveDataInterface.h"
#include "DWGameMode.generated.h"

/**
 * 游戏模式基类
 */
UCLASS()
class DREAMWORLD_API ADWGameMode : public AWHGameMode, public ISaveDataInterface
{
private:
	GENERATED_BODY()

public:
	ADWGameMode();
	
	//////////////////////////////////////////////////////////////////////////
	/// WHActor
public:
	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation(EPhase InPhase) override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnTermination_Implementation(EPhase InPhase) override;

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual void UnloadData(EPhase InPhase) override;

	virtual FSaveData* ToData() override;

protected:
	UPROPERTY(EditAnywhere, Category = "GameSetting")
	EDWGameLevel GameLevel;

	UPROPERTY(EditAnywhere, Category = "GameSetting")
	bool bAutoJump;

public:
	UFUNCTION(BlueprintPure)
	EDWGameLevel GetGameLevel() const { return GameLevel; }

	UFUNCTION(BlueprintCallable)
	void SetGameLevel(EDWGameLevel InGameLevel) { GameLevel = InGameLevel; }

	UFUNCTION(BlueprintPure)
	bool IsAutoJump() const { return bAutoJump; }

	UFUNCTION(BlueprintCallable)
	void SetAutoJump(bool bInAutoJump) { bAutoJump = bInAutoJump; }
};
