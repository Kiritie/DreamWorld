// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/DWCommonTypes.h"
#include "Setting/SettingModule.h"

#include "DWSettingModule.generated.h"

UCLASS()
class DREAMWORLD_API UDWSettingModule : public USettingModule
{
	GENERATED_BODY()
	
	GENERATED_MODULE(UDWSettingModule)

public:	
	// ParamSets default values for this actor's properties
	UDWSettingModule();
	
	~UDWSettingModule();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
#endif

	virtual void OnInitialize() override;

	virtual void OnPreparatory(EPhase InPhase) override;

	virtual void OnRefresh(float DeltaSeconds, bool bInEditor) override;

	virtual void OnPause() override;

	virtual void OnUnPause() override;

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual void UnloadData(EPhase InPhase) override;

	virtual FSaveData* ToData() override;

protected:
	UPROPERTY(EditAnywhere, Category = "GameSetting")
	EDWGameLevel GameLevel;

	UPROPERTY(EditAnywhere, Category = "GameSetting")
	bool bAutoJump;

	UPROPERTY(EditAnywhere, Category = "GameSetting")
	bool bAutoJumpWithCreating;

public:
	UFUNCTION(BlueprintPure)
	EDWGameLevel GetGameLevel() const { return GameLevel; }

	UFUNCTION(BlueprintCallable)
	void SetGameLevel(EDWGameLevel InGameLevel) { GameLevel = InGameLevel; }

	UFUNCTION(BlueprintPure)
	bool IsAutoJump(bool bCheckCreating = false) const;

	UFUNCTION(BlueprintCallable)
	void SetAutoJump(bool bInAutoJump) { bAutoJump = bInAutoJump; }

	UFUNCTION(BlueprintPure)
	bool IsAutoJumpWithCreating() const { return bAutoJumpWithCreating; }

	UFUNCTION(BlueprintCallable)
	void SetAutoJumpWithCreating(bool bInAutoJumpWithCreating) { bAutoJumpWithCreating = bInAutoJumpWithCreating; }
};
