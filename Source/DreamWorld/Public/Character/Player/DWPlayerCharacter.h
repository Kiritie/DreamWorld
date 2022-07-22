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
	
	friend class ADWPlayerController;

	friend class UDWPlayerCharacterState_Attack;
	friend class UDWPlayerCharacterState_Death;
	friend class UDWPlayerCharacterState_Default;
	friend class UDWPlayerCharacterState_Dodge;
	friend class UDWPlayerCharacterState_Interrupt;

public:
	ADWPlayerCharacter();

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation() override;

	virtual void LoadData(FSaveData* InSaveData, bool bForceMode) override;

	virtual FSaveData* ToData() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void LookAtTarget(ADWCharacter* InTargetCharacter) override;

	virtual bool UseItem(FAbilityItem& InItem) override;

	virtual void RefreshEquip(EDWEquipPartType InPartType, UInventoryEquipSlot* EquipSlot) override;

	virtual void OnEnterInteract(IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnLeaveInteract(IInteractionAgentInterface* InInteractionAgent) override;

protected:
	virtual void MoveForward_Implementation(float InValue) override;

	virtual void MoveRight_Implementation(float InValue) override;

	virtual void MoveUp_Implementation(float InValue) override;

public:
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;

public:
	virtual void SetNameV(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual void SetLevelV(int32 InLevel) override;

	virtual void SetEXP(int32 InEXP) override;

	virtual void SetTeamID(FName InTeamID) override;
	
	virtual FString GetHeadInfo() const override;
		
	virtual void SetControlMode(EDWCharacterControlMode InControlMode) override;

	virtual void SetGenerateVoxelItem(FVoxelItem InGenerateVoxelItem) override;
	
	UFUNCTION(BlueprintPure)
	UTargetSystemComponent* GetTargetSystem() const { return TargetSystem; }

	virtual UCameraComponent* GetCameraComp_Implementation() override { return FollowCamera; }

	virtual USpringArmComponent* GetCameraBoom_Implementation() override { return CameraBoom; }
};
