// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.ADWCharacter

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Character/Human/DWHumanCharacter.h"
#include "Gameplay/WHPlayerInterface.h"
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
class UTargetSystemComponent;

/**
 * ��ҽ�ɫ
 */
UCLASS()
class ADWPlayerCharacter : public ADWHumanCharacter, public IWHPlayerInterface
{
	GENERATED_BODY()

	friend class UDWPlayerCharacterState_Attack;
	friend class UDWPlayerCharacterState_Death;
	friend class UDWPlayerCharacterState_Default;
	friend class UDWPlayerCharacterState_Dodge;
	friend class UDWPlayerCharacterState_Interrupt;

public:
	ADWPlayerCharacter();

protected:
	// states

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStates")
	EDWControlMode ControlMode;
	
	// stats
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStats")
	FVoxelItem VoxelItem;

	// inputs
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterInputs")
	bool bPressedAttack;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterInputs")
	bool bPressedDefend;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTargetSystemComponent* TargetSystem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FollowCamera;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVoxelMeshComponent* VoxelMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* HammerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneCaptureComponent2D* PreviewCapture;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneCaptureComponent2D* MiniMapCapture;

protected:
	int32 AttackAbilityQueue;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void LoadData(FSaveData* InSaveData) override;

	virtual FSaveData* ToData() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void LookAtTarget(ADWCharacter* InTargetCharacter) override;

	virtual bool UseItem(FAbilityItem& InItem) override;

	virtual void RefreshEquip(EDWEquipPartType InPartType, UInventoryEquipSlot* EquipSlot) override;

	UFUNCTION(BlueprintCallable)
	virtual void UpdateVoxelMesh();
	
	virtual bool RaycastVoxel(FVoxelHitResult& OutHitResult) override;

	virtual void OnEnterInteract(IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnLeaveInteract(IInteractionAgentInterface* InInteractionAgent) override;

protected:
	virtual void MoveForward_Implementation(float InValue) override;

	virtual void MoveRight_Implementation(float InValue) override;

	virtual void MoveUp_Implementation(float InValue) override;
	
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

public:
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;

public:
	virtual void SetNameV(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual void SetLevelV(int32 InLevel) override;

	virtual void SetEXP(int32 InEXP) override;

	virtual void SetTeamID(FName InTeamID) override;
	
	virtual FString GetHeadInfo() const override;

	UFUNCTION(BlueprintPure)
	EDWControlMode GetControlMode() const { return ControlMode; }
		
	UFUNCTION(BlueprintCallable)
	virtual void SetControlMode(EDWControlMode InControlMode);
	
	UFUNCTION(BlueprintPure)
	UTargetSystemComponent* GetTargetSystem() const { return TargetSystem; }

	virtual UCameraComponent* GetCameraComp_Implementation() override { return FollowCamera; }

	virtual USpringArmComponent* GetCameraBoom_Implementation() override { return CameraBoom; }
};
