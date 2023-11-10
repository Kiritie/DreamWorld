// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/InputModule.h"

#include "DWInputModule.generated.h"

UCLASS()
class DREAMWORLD_API UDWInputModule : public UInputModule
{
	GENERATED_BODY()
	
	GENERATED_MODULE(UDWInputModule)

public:	
	// ParamSets default values for this actor's properties
	UDWInputModule();
	
	~UDWInputModule();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
#endif

	virtual void OnInitialize() override;

	virtual void OnPreparatory(EPhase InPhase) override;

	virtual void OnReset() override;

	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnPause() override;

	virtual void OnUnPause() override;

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual void UnloadData(EPhase InPhase) override;

	virtual FSaveData* ToData() override;

protected:
	virtual void OnBindAction_Implementation(UInputComponentBase* InInputComponent, UPlayerMappableInputConfig* InInputConfig) override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "InputStates|Player")
	bool bPressedAttackDestroy;
		
	UPROPERTY(VisibleAnywhere, Category = "InputStates|Player")
	bool bPressedDefendGenerate;

	UPROPERTY(VisibleAnywhere, Category = "InputStates|Player")
	bool bPressedSprint;

protected:
	virtual void TurnPlayer(const FInputActionValue& InValue) override;
	
	virtual void MoveHPlayer(const FInputActionValue& InValue) override;

	virtual void MoveVPlayer(const FInputActionValue& InValue) override;

	virtual void MoveForwardPlayer(const FInputActionValue& InValue) override;

	virtual void MoveRightPlayer(const FInputActionValue& InValue) override;
	
	virtual void MoveUpPlayer(const FInputActionValue& InValue) override;
	
protected:
	UFUNCTION()
	virtual void OnSprintPressed();

	UFUNCTION()
	virtual void OnSprintReleased();
	
	UFUNCTION()
	virtual void ToggleControlMode();

	UFUNCTION()
	virtual void ToggleCrouch();

	UFUNCTION()
	virtual void ToggleLockTarget();
	
	UFUNCTION()
	virtual void ChangeHand();

	UFUNCTION()
	virtual void OnDodgePressed();

	UFUNCTION()
	virtual void OnDodgeReleased();

	UFUNCTION()
	virtual void OnAttackDestroyPressed();

	UFUNCTION()
	virtual void OnAttackDestroyReleased();

	UFUNCTION()
	virtual void OnDefendGeneratePressed();

	UFUNCTION()
	virtual void OnDefendGenerateReleased();

	UFUNCTION()
	virtual void ReleaseSkillAbility1();

	UFUNCTION()
	virtual void ReleaseSkillAbility2();

	UFUNCTION()
	virtual void ReleaseSkillAbility3();

	UFUNCTION()
	virtual void ReleaseSkillAbility4();

	UFUNCTION()
	virtual void DoInteractAction1();

	UFUNCTION()
	virtual void DoInteractAction2();

	UFUNCTION()
	virtual void DoInteractAction3();

	UFUNCTION()
	virtual void DoInteractAction4();

	UFUNCTION()
	virtual void DoInteractAction5();

	UFUNCTION()
	virtual void OpenInventoryPanel();
	
	UFUNCTION()
	virtual void OpenGeneratePanel();

	UFUNCTION()
	virtual void UseInventoryItem();

	UFUNCTION()
	virtual void DiscardInventoryItem();

	UFUNCTION()
	virtual void PrevInventorySlot();

	UFUNCTION()
	virtual void NextInventorySlot();

	UFUNCTION()
	virtual void SelectInventorySlot1();
		
	UFUNCTION()
	virtual void SelectInventorySlot2();
		
	UFUNCTION()
	virtual void SelectInventorySlot3();
		
	UFUNCTION()
	virtual void SelectInventorySlot4();
		
	UFUNCTION()
	virtual void SelectInventorySlot5();
		
	UFUNCTION()
	virtual void SelectInventorySlot6();
		
	UFUNCTION()
	virtual void SelectInventorySlot7();
		
	UFUNCTION()
	virtual void SelectInventorySlot8();
		
	UFUNCTION()
	virtual void SelectInventorySlot9();
		
	UFUNCTION()
	virtual void SelectInventorySlot10();

	UFUNCTION()
	virtual void PauseGame();
};
