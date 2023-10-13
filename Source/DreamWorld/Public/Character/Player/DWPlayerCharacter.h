// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.ADWCharacter

#pragma once

#include "Character/Human/DWHumanCharacter.h"
#include "DWPlayerCharacter.generated.h"

class UVoxel;
class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UVoxelMeshComponent;
class USkeletalMeshComponent;
class USceneCaptureComponent2D;
class ADWPlayerController;
class UPawnSensingComponent;
class ADWPlayerCharacterCameraManager;
class UTargetingComponent;

/**
 * ��ҽ�ɫ
 */
UCLASS()
class DREAMWORLD_API ADWPlayerCharacter : public ADWHumanCharacter
{
	GENERATED_BODY()
	
	friend class ADWPlayerController;
	friend class ADWInputModule;

	friend class UDWPlayerCharacterState_Attack;
	friend class UDWPlayerCharacterState_Death;
	friend class UDWPlayerCharacterState_Default;
	friend class UDWPlayerCharacterState_Dodge;
	friend class UDWPlayerCharacterState_Interrupt;

public:
	ADWPlayerCharacter();

public:
	virtual void OnInitialize_Implementation() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTargetingComponent* Targeting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneCaptureComponent2D* PreviewCapture;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneCaptureComponent2D* MiniMapCapture;

protected:
	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData(bool bRefresh) override;

public:
	virtual void Death(IAbilityVitalityInterface* InKiller /* = nullptr */) override;

	virtual void Kill(IAbilityVitalityInterface* InTarget) override;

	virtual bool CanLookAtTarget(ADWCharacter* InTargetCharacter) override;

	virtual void DoLookAtTarget(ADWCharacter* InTargetCharacter) override;

	virtual void ChangeHand();

	virtual bool CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnEnterInteract(IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnLeaveInteract(IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassivity) override;

	virtual bool OnInteractVoxel(const FVoxelHitResult& InVoxelHitResult, EInputInteractAction InInteractAction) override;

protected:
	virtual void Turn_Implementation(float InValue) override;

	virtual void MoveForward_Implementation(float InValue) override;

	virtual void MoveRight_Implementation(float InValue) override;

	virtual void MoveUp_Implementation(float InValue) override;

public:
	virtual void OnAssembleItem(const FAbilityItem& InItem) override;

	virtual void OnDischargeItem(const FAbilityItem& InItem) override;
	
	virtual void OnSelectItem(const FAbilityItem& InItem) override;
	
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;

public:
	virtual void SetNameV(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual bool SetLevelV(int32 InLevel) override;

	virtual void SetTeamID(FName InTeamID) override;
	
	virtual FString GetHeadInfo() const override;

	virtual void SetActorVisible_Implementation(bool bInVisible) override;
		
	virtual void SetControlMode_Implementation(EDWCharacterControlMode InControlMode) override;

	virtual void SetGenerateVoxelID(const FPrimaryAssetId& InGenerateVoxelID) override;
	
	UFUNCTION()
	virtual void OnTargetLockedOn(AActor* InTargetActor);
		
	UFUNCTION()
	virtual void OnTargetLockedOff(AActor* InTargetActor);

	UFUNCTION()
	virtual void OnTargetSetRotation(AActor* InTargetActor, FRotator InControlRotation);

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	int32 GetBodyColor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	int32 GetCapeColor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetBodyColor(int32 InColorIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCapeColor(int32 InColorIndex);

public:
	UFUNCTION(BlueprintPure)
	virtual UTargetingComponent* GetTargeting() const { return Targeting; }
};
