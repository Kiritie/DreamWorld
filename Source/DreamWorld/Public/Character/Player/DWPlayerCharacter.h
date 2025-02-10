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
	friend class UDWInputManager;

	friend class UDWPlayerCharacterState_Attack;
	friend class UDWPlayerCharacterState_Death;
	friend class UDWPlayerCharacterState_Spawn;
	friend class UDWPlayerCharacterState_Dodge;
	friend class UDWPlayerCharacterState_Interrupt;

public:
	ADWPlayerCharacter(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnInitialize_Implementation() override;

protected:
	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

public:
	virtual void Death(IAbilityVitalityInterface* InKiller /* = nullptr */) override;

	virtual void Kill(IAbilityVitalityInterface* InTarget) override;

	virtual void ChangeHand();
	
	virtual bool CanLookAtTarget() override;

	virtual void OnTargetLookAtOn(AActor* InTargetActor) override;

	virtual void OnTargetLookAtOff(AActor* InTargetActor) override;

	virtual bool CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnEnterInteract(IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnLeaveInteract(IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive) override;

	virtual bool OnInteractVoxel(EInputInteractAction InInteractAction, EInputInteractEvent InInteractEvent, const FVoxelHitResult& InHitResult) override;

protected:
	virtual void MoveForward_Implementation(float InValue) override;

	virtual void MoveRight_Implementation(float InValue) override;

	virtual void MoveUp_Implementation(float InValue) override;

public:
	virtual void OnAdditionItem(const FAbilityItem& InItem) override;

	virtual void OnChangeItem(const FAbilityItem& InNewItem) override;
	
	virtual void OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess) override;

	virtual void OnDeactiveItem(const FAbilityItem& InItem, bool bPassive) override;

	virtual void OnSelectItem(const FAbilityItem& InItem) override;
	
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;

	virtual void OnActorAttached(AActor* InActor) override;

	virtual void OnActorDetached(AActor* InActor) override;
	
public:
	UFUNCTION()
	virtual void OnTargetLockedOn(AActor* InTargetActor);
		
	UFUNCTION()
	virtual void OnTargetLockedOff(AActor* InTargetActor);

	UFUNCTION()
	virtual void OnTargetSetRotation(AActor* InTargetActor, FRotator InControlRotation);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UTargetingComponent* Targeting;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneCaptureComponent2D* PreviewCapture;

public:
	UFUNCTION(BlueprintPure)
	virtual UTargetingComponent* GetTargeting() const { return Targeting; }

public:
	virtual void SetNameA(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual bool SetLevelA(int32 InLevel) override;

	virtual void SetTeamID(FName InTeamID) override;
	
	virtual FString GetHeadInfo() const override;

	virtual void SetActorVisible_Implementation(bool bInVisible) override;
		
	virtual void SetControlMode(EDWCharacterControlMode InControlMode) override;

	virtual void SetGenerateVoxelID(const FPrimaryAssetId& InGenerateVoxelID) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	int32 GetBodyColor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	int32 GetCapeColor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetBodyColor(int32 InColorIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCapeColor(int32 InColorIndex);
};
