// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Gameplay/WHPlayerController.h"
#include "SaveGame/Base/SaveDataInterface.h"

#include "DWPlayerController.generated.h"

/**
 * 玩家角色控制器
 */
UCLASS()
class DREAMWORLD_API ADWPlayerController : public AWHPlayerController, public ISaveDataInterface
{
	GENERATED_BODY()

	friend class ADWCharacter;
	friend class ADWPlayerCharacter;

public:
	ADWPlayerController();

public:
	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation() override;

protected:
	virtual void LoadData(FSaveData* InSaveData, bool bForceMode) override;

	virtual FSaveData* ToData() override;

	virtual void UnloadData(bool bForceMode) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inputs")
	bool bPressedAttack;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inputs")
	bool bPressedDefend;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inputs")
	bool bPressedSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inputs")
	int32 AttackAbilityQueue;

protected:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual bool RaycastFromAimPoint(FHitResult& OutHitResult, EDWGameTraceType InGameTraceType, float InRayDistance = 1000.f) const;
	
	virtual bool RaycastVoxel(FVoxelHitResult& OutHitResult);

protected:
	virtual void OnJumpPressed(FKey Key);

	virtual void OnJumpReleased();

	virtual void OnSprintPressed();

	virtual void OnSprintReleased();
	
	virtual void ToggleControlMode();

	virtual void ToggleCrouch();

	virtual void ToggleLockTarget();

	virtual void OnDodgePressed();

	virtual void OnDodgeReleased();

	virtual void OnAttackDestroyPressed();

	virtual void OnAttackDestroyRepeat();

	virtual void OnAttackDestroyReleased();

	virtual void OnDefendGeneratePressed();

	virtual void OnDefendGenerateRepeat();

	virtual void OnDefendGenerateReleased();

	virtual void ReleaseSkillAbility1();

	virtual void ReleaseSkillAbility2();

	virtual void ReleaseSkillAbility3();

	virtual void ReleaseSkillAbility4();

	virtual void DoInteractAction1();

	virtual void DoInteractAction2();

	virtual void DoInteractAction3();

	virtual void DoInteractAction4();

	virtual void DoInteractAction5();

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
