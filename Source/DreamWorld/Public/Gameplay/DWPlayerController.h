// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Gameplay/WHPlayerController.h"

#include "DWPlayerController.generated.h"

/**
 * 玩家角色控制器
 */
UCLASS()
class DREAMWORLD_API ADWPlayerController : public AWHPlayerController
{
	GENERATED_BODY()

public:
	ADWPlayerController();

public:
	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inputs")
	bool bPressedSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inputs")
	float DoubleJumpTime;

protected:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DeltaTime) override;

	void LoadData(FDWPlayerSaveData InPlayerData);

	void UnloadData();

	void ResetData();

	bool RaycastFromAimPoint(FHitResult& OutHitResult, EDWGameTraceType InGameTraceType, float InRayDistance = 1000) const;

protected:
	virtual void OnJumpPressed(FKey Key);

	virtual void OnJumpReleased();

	virtual void OnSprintPressed();

	virtual void OnSprintReleased();

	virtual void ToggleInventoryPanel();

	virtual void UseInventoryItem();

	virtual void UseAllInventoryItem();

	virtual void DiscardInventoryItem();

	virtual void DiscardAllInventoryItem();

	virtual void PrevInventorySlot();

	virtual void NextInventorySlot();

	virtual void SelectInventorySlot1();
		
	virtual void SelectInventorySlot2();
		
	virtual void SelectInventorySlot3();
		
	virtual void SelectInventorySlot4();
		
	virtual void SelectInventorySlot5();
		
	virtual void SelectInventorySlot6();
		
	virtual void SelectInventorySlot7();
		
	virtual void SelectInventorySlot8();
		
	virtual void SelectInventorySlot9();
		
	virtual void SelectInventorySlot10();

	virtual void PauseOrContinueGame();
};
