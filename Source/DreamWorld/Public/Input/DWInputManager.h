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

	virtual void OnReset() override;

	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnBindAction(UInputComponentBase* InInputComponent) override;

protected:
	virtual void OnTurnCamera_Implementation(const FInputActionValue& InValue) override;

	virtual void OnTurnPlayer_Implementation(const FInputActionValue& InValue) override;

	virtual void OnMoveForwardPlayer_Implementation(const FInputActionValue& InValue) override;

	virtual void OnMoveRightPlayer_Implementation(const FInputActionValue& InValue) override;
	
	virtual void OnMoveUpPlayer_Implementation(const FInputActionValue& InValue) override;

	virtual void OnActionUpPlayer_Implementation(const FInputActionValue& InValue) override;

	virtual void SystemOperation_Implementation() override;
	
protected:
	virtual void OnPrimaryPressed_Implementation() override;

	virtual void OnPrimaryRepeated_Implementation() override;

	virtual void OnPrimaryReleased_Implementation() override;

	virtual void OnSecondaryPressed_Implementation() override;

	virtual void OnSecondaryRepeated_Implementation() override;

	virtual void OnSecondaryReleased_Implementation() override;

	virtual void OnThirdPressed_Implementation() override;

	virtual void OnThirdRepeated_Implementation() override;

	virtual void OnThirdReleased_Implementation() override;

	UFUNCTION()
	virtual void OnSprintPressed();

	UFUNCTION()
	virtual void OnSprintReleased();

	UFUNCTION()
	virtual void OnDodgePressed();

	UFUNCTION()
	virtual void OnDodgeReleased();

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
	virtual void NextInteract();

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
		
	UFUNCTION(BlueprintCallable)
	virtual void OpenTaskPanel();

	UFUNCTION(BlueprintCallable)
	virtual void OpenContextInputBox();

	UFUNCTION(BlueprintCallable)
	virtual void OpenSwitcherPanel();

protected:
	UPROPERTY(VisibleAnywhere)
	bool bPrimaryPressed;
		
	UPROPERTY(VisibleAnywhere)
	bool bSecondaryPressed;
		
	UPROPERTY(VisibleAnywhere)
	bool bThirdPressed;

	UPROPERTY(VisibleAnywhere)
	bool bSprintPressed;

	UPROPERTY(VisibleAnywhere)
	int32 AttackAbilityQueue;
};
