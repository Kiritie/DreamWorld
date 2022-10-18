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
	virtual void Death(IAbilityVitalityInterface* InKiller /* = nullptr */) override;

	virtual void Kill(IAbilityVitalityInterface* InTarget) override;

	virtual bool CanLookAtTarget(ADWCharacter* InTargetCharacter) override;

	virtual void LookAtTarget(ADWCharacter* InTargetCharacter) override;

	virtual void RefreshEquip(EDWEquipPartType InPartType, const FAbilityItem& InItem) override;

	virtual void OnEnterInteract(IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnLeaveInteract(IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual void ChangeHand();

protected:
	virtual void Turn_Implementation(float InValue) override;

	virtual void MoveForward_Implementation(float InValue) override;

	virtual void MoveRight_Implementation(float InValue) override;

	virtual void MoveUp_Implementation(float InValue) override;

public:
	virtual void OnSelectItem(const FAbilityItem& InItem) override;
	
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;

public:
	virtual void SetNameV(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual bool SetLevelV(int32 InLevel) override;

	virtual void SetTeamID(FName InTeamID) override;
	
	virtual FString GetHeadInfo() const override;

	virtual void SetActorVisible_Implementation(bool bNewVisible) override;
		
	virtual void SetControlMode(EDWCharacterControlMode InControlMode) override;

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
	virtual UTargetSystemComponent* GetTargetSystem() const { return TargetSystem; }

	virtual UCameraComponent* GetCameraComp_Implementation() override { return FollowCamera; }

	virtual USpringArmComponent* GetCameraBoom_Implementation() override { return CameraBoom; }
};
