// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/Manager/DefaultInputManagerBase.h"

#include "DWInputManager.generated.h"

UCLASS()
class DREAMWORLD_API UDWInputManager : public UDefaultInputManagerBase
{
	GENERATED_BODY()

public:	
	// ParamSets default values for this actor's properties
	UDWInputManager();

public:
	virtual void OnInitialize() override;

	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnBindAction(UInputComponentBase* InInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "InputStates|Player")
	bool bPressedAttackDestroy;
		
	UPROPERTY(VisibleAnywhere, Category = "InputStates|Player")
	bool bPressedDefendGenerate;

	UPROPERTY(VisibleAnywhere, Category = "InputStates|Player")
	bool bPressedSprint;

	UPROPERTY(VisibleAnywhere, Category = "InputStates|Player")
	int32 AttackAbilityQueue;

protected:
	virtual void TurnPlayer_Implementation(const FInputActionValue& InValue) override;

	virtual void MoveForwardPlayer_Implementation(const FInputActionValue& InValue) override;

	virtual void MoveRightPlayer_Implementation(const FInputActionValue& InValue) override;
	
	virtual void MoveUpPlayer_Implementation(const FInputActionValue& InValue) override;

	virtual void ActionUpPlayer_Implementation(const FInputActionValue& InValue) override;

	virtual void SystemOperation_Implementation() override;
	
protected:
	UFUNCTION()
	virtual void OnSprintPressed();

	UFUNCTION()
	virtual void OnSprintReleased();

	UFUNCTION()
	virtual void OnDodgePressed();

	UFUNCTION()
	virtual void OnDodgeReleased();

	UFUNCTION()
	virtual void OnPrimaryPressed();

	UFUNCTION()
	virtual void OnPrimaryReleased();

	UFUNCTION()
	virtual void OnSecondaryPressed();

	UFUNCTION()
	virtual void OnSecondaryReleased();

	UFUNCTION()
	virtual void OnThirdPressed();

	UFUNCTION()
	virtual void OnThirdReleased();

public:
	UFUNCTION(BlueprintCallable)
	virtual void ToggleControlMode();

	UFUNCTION(BlueprintCallable)
	virtual void ToggleCrouch();
	
	UFUNCTION(BlueprintCallable)
	virtual void ChangeHand();

	UFUNCTION(BlueprintCallable)
	virtual void ReleaseSkillAbility1();

	UFUNCTION(BlueprintCallable)
	virtual void ReleaseSkillAbility2();

	UFUNCTION(BlueprintCallable)
	virtual void ReleaseSkillAbility3();

	UFUNCTION(BlueprintCallable)
	virtual void ReleaseSkillAbility4();

	UFUNCTION(BlueprintCallable)
	virtual void DoInteract1();

	UFUNCTION(BlueprintCallable)
	virtual void DoInteract2();

	UFUNCTION(BlueprintCallable)
	virtual void DoInteract3();

	UFUNCTION(BlueprintCallable)
	virtual void DoInteract4();

	UFUNCTION(BlueprintCallable)
	virtual void DoInteract5();

	UFUNCTION(BlueprintCallable)
	virtual void UseInventoryItem();

	UFUNCTION(BlueprintCallable)
	virtual void DiscardInventoryItem();

	UFUNCTION(BlueprintCallable)
	virtual void PrevInventorySlot();

	UFUNCTION(BlueprintCallable)
	virtual void NextInventorySlot();

	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot1();
		
	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot2();
		
	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot3();
		
	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot4();
		
	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot5();
		
	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot6();
		
	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot7();
		
	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot8();
		
	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot9();
		
	UFUNCTION(BlueprintCallable)
	virtual void SelectInventorySlot10();

	UFUNCTION(BlueprintCallable)
	virtual void ZoomInMiniMap();

	UFUNCTION(BlueprintCallable)
	virtual void ZoomOutMiniMap();

	UFUNCTION(BlueprintCallable)
	virtual void OpenMaxMapBox();

	UFUNCTION(BlueprintCallable)
	virtual void OpenInventoryPanel();
	
	UFUNCTION(BlueprintCallable)
	virtual void OpenGeneratePanel();
};
