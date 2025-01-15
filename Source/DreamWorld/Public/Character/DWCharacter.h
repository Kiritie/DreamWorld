// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Ability/Character/AbilityCharacterBase.h"
#include "Ability/Attributes/DWCharacterAttributeSet.h"
#include "Ability/Hitter/AbilityHitterInterface.h"
#include "Common/DWCommonTypes.h"
#include "Team/Agent/DWTeamAgentInterface.h"

#include "DWCharacter.generated.h"

class AVoxelInteractAuxiliary;
class UDialogue;
class ADWEquip;
class UEventHandle_VoxelWorldModeChanged;
class UWorldWidgetComponent;
class UDWCharacterData;
class ADWVoxelChunk;
class UInteractionComponent;
class UInteractionComponent;
class UDWCharacterPart;
class ADWEquipArmor;
class ADWEquipShield;
class ADWEquipWeapon;
class UAbilityInventoryEquipSlotBase;
class AVoxelChunk;
class UVoxel;
class AController;
class UDWCharacterAnim;
class UAbilityBase;
class UAbilitySystemComponentBase;
class UAbilityCharacterInventoryBase;
class UBehaviorTree;
class UDWAIBlackboard;
class UAbilityInventorySlotBase;
class AAbilityProjectileBase;

/**
 * 角色
 */
UCLASS()
class DREAMWORLD_API ADWCharacter : public AAbilityCharacterBase, public IAbilityHitterInterface, public IDWTeamAgentInterface
{
	GENERATED_BODY()

	friend class ADWPlayerController;
	
	friend class UDWCharacterState_Aim;
	friend class UDWCharacterState_Attack;
	friend class UDWCharacterState_Death;
	friend class UDWCharacterState_Spawn;
	friend class UDWCharacterState_Defend;
	friend class UDWCharacterState_Dodge;
	friend class UDWCharacterState_Float;
	friend class UDWCharacterState_Fly;
	friend class UDWCharacterState_Ride;

public:
	ADWCharacter(const FObjectInitializer& ObjectInitializer);

public:
	virtual int32 GetLimit_Implementation() const override { return 0; }

	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	virtual void OnInitialize_Implementation() override;
	
	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void ResetData() override;

	virtual void SetActorVisible_Implementation(bool bInVisible) override;

	virtual void OnFiniteStateRefresh(UFiniteStateBase* InCurrentState) override;
	
public:
	virtual void Serialize(FArchive& Ar) override;

public:
	virtual void Death(IAbilityVitalityInterface* InKiller) override;
			
	virtual void Revive(IAbilityVitalityInterface* InRescuer) override;

	UFUNCTION(BlueprintCallable)
	virtual void Dodge();

	UFUNCTION(BlueprintCallable)
	virtual void UnDodge();

	UFUNCTION(BlueprintCallable)
	virtual void Sprint();

	UFUNCTION(BlueprintCallable)
	virtual void UnSprint();

	UFUNCTION(BlueprintCallable)
	virtual void Ride(ADWCharacter* InTarget);

	UFUNCTION(BlueprintCallable)
	virtual void UnRide();

	UFUNCTION(BlueprintCallable)
	virtual void Sleep(AVoxelInteractAuxiliary* InBed);

	UFUNCTION(BlueprintCallable)
	virtual void UnSleep();

	UFUNCTION(BlueprintCallable)
	virtual void Aim();

	UFUNCTION(BlueprintCallable)
	virtual void UnAim();

	virtual bool Attack(EDWWeaponPart InWeaponPart, int32 InAbilityIndex = -1, const FSimpleDelegate& OnCompleted = nullptr);

	virtual bool FallingAttack(EDWWeaponPart InWeaponPart, const FSimpleDelegate& OnCompleted = nullptr);

	virtual bool Skill(const FPrimaryAssetId& InSkillID);

	virtual bool Skill(int32 InSkillIndex = 0);

	virtual bool Skill(EDWSkillType InSkillType, int32 InAbilityIndex = -1);
	
	virtual bool SkillAttack(const FAbilityItem& InAbilityItem, const FSimpleDelegate& OnCompleted = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void UnAttack();

	UFUNCTION(BlueprintCallable)
	virtual void Defend();
	
	UFUNCTION(BlueprintCallable)
	virtual void UnDefend();

	virtual bool DoAction(const FGameplayTag& InActionTag) override;

	virtual bool StopAction(const FGameplayTag& InActionTag) override;

	virtual void EndAction(const FGameplayTag& InActionTag, bool bWasCancelled) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void AIMoveTo(FVector InTargetLocation, float InMoveStopDistance = 10.f, bool bMulticast = false) override;

	UFUNCTION(BlueprintCallable)
	virtual bool DoAIMove(FVector InTargetLocation, float InMoveStopDistance = 10.f);
	
	virtual bool DoAIMove(AActor* InTargetActor, float InMoveStopDistance = 10.f, bool bLookAtTarget = false);

	virtual void StopAIMove(bool bMulticast = false) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
									
	virtual void SetMotionRate(float InMovementRate, float InRotationRate) override;

	virtual bool RaycastStep(FHitResult& OutHitResult);

	virtual bool CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive) override;

	virtual void OnAdditionItem(const FAbilityItem& InItem) override;

	virtual void OnPreChangeItem(const FAbilityItem& InOldItem) override;

	virtual void OnChangeItem(const FAbilityItem& InNewItem) override;

	virtual void OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess) override;

	virtual void OnRemoveItem(const FAbilityItem& InItem) override;
		
	virtual void OnDeactiveItem(const FAbilityItem& InItem, bool bPassive) override;

	virtual void OnDiscardItem(const FAbilityItem& InItem, bool bInPlace) override;

	virtual void OnSelectItem(const FAbilityItem& InItem) override;

	virtual void OnAuxiliaryItem(const FAbilityItem& InItem) override;

	virtual void OnPickUp(AAbilityPickUpBase* InPickUp) override;

	virtual bool OnGenerateVoxel(EInputInteractEvent InInteractEvent, const FVoxelHitResult& InHitResult) override;

	virtual bool OnDestroyVoxel(EInputInteractEvent InInteractEvent, const FVoxelHitResult& InHitResult) override;

protected:
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;

	UFUNCTION()
	virtual void OnWorldModeChanged(UObject* InSender, UEventHandle_VoxelWorldModeChanged* InEventHandle);

public:
	virtual void HandleDamage(const FGameplayAttribute& DamageAttribute, float DamageValue, float DefendValue, bool bHasCrited, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;

	virtual void HandleRecovery(const FGameplayAttribute& RecoveryAttribute, float RecoveryValue, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;

	virtual void HandleInterrupt(const FGameplayAttribute& InterruptAttribute, float InterruptDuration, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;

public:
	virtual bool IsTargetAble_Implementation(APawn* InPlayerPawn) const override;

	virtual bool IsLookAtAble_Implementation(AActor* InLookerActor) const override;

	virtual bool CanLookAtTarget() override;

	virtual bool IsHitAble() const override;

	virtual void SetHitAble(bool bValue) override;
		
	virtual void ClearHitTargets() override;

	virtual TArray<AActor*> GetHitTargets() const override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWorldWidgetComponent* CharacterHP;

protected:
	// stats
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStates")
	EDWCharacterControlMode ControlMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	FName TeamID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStats")
	bool bHitAble;

protected:
	float AIMoveStopDistance;

	FVector AIMoveLocation;
	
	float CameraDoRotationTime;
	float CameraDoRotationDuration;
	FRotator CameraDoRotationRotation;

	EDWCharacterAttackType AttackType;

	EDWWeaponPart AttackWeaponPart;

	FAbilityItem SkillAttackAbilityItem;

	UPROPERTY()
	AAbilityProjectileBase* AttackProjectile;

	UPROPERTY(VisibleAnywhere)
	UDialogue* Dialogue;

	UPROPERTY()
	TMap<EDWEquipPart, ADWEquip*> Equips;

	TMap<EDWWeaponType, FDWCharacterAttackAbilityQueue> AttackAbilityQueues;
	
	TMap<EDWWeaponType, FDWCharacterFallingAttackAbilityData> FallingAttackAbilities;

	TMap<FPrimaryAssetId, FDWCharacterSkillAttackAbilityData> SkillAttackAbilities;

public:
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Mana)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MaxMana)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ManaRecovery)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ManaRegenSpeed)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Stamina)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MaxStamina)
			
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaRecovery)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaRegenSpeed)
			
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaExpendSpeed)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DodgeForce)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, RepulseForce)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackForce)
		
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackSpeed)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackCritRate)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackStealRate)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DefendRate)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DefendScopeRate)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, PhysicsRes)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MagicRes)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ToughnessRate)

public:
	virtual bool IsEnemy(IAbilityPawnInterface* InTarget) const override;
	
	UFUNCTION(BlueprintPure, DisplayName = "IsEnemy")
	virtual bool IsEnemyN(ADWCharacter* InTarget);
	
	UFUNCTION(BlueprintPure)
	virtual bool IsExhausted() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsDodging() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsSprinting() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsAiming() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsAttacking(EDWCharacterAttackType InAttackType = EDWCharacterAttackType::None) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsDefending() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsRiding() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsSleeping() const;

public:
	virtual FGuid GetActorIDT() const override { return ActorID; }
	
	virtual FName GetNameT() const override { return Name; }
	
	virtual void SetNameA(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual bool SetLevelA(int32 InLevel) override;

	UFUNCTION(BlueprintPure)
	EDWCharacterControlMode GetControlMode() const { return ControlMode; }

	UFUNCTION(BlueprintNativeEvent)
	void SetControlMode(EDWCharacterControlMode InControlMode);

	UFUNCTION(BlueprintPure)
	UWorldWidgetComponent* GetCharacterHP() const { return CharacterHP; }

	UFUNCTION(BlueprintPure)
	UWidgetCharacterHP* GetCharacterHPWidget() const;

	UFUNCTION(BlueprintPure)
	virtual FName GetTeamID() const override { return TeamID; }

	UFUNCTION(BlueprintCallable)
	virtual void SetTeamID(FName InTeamID) override;
	
	UFUNCTION(BlueprintPure)
	EDWCharacterNature GetNature() const;

	UFUNCTION(BlueprintPure)
	float GetAttackDistance() const;
	
	UFUNCTION(BlueprintPure)
	float GetInteractDistance() const;

	UFUNCTION(BlueprintPure)
	float GetFollowDistance() const;

	UFUNCTION(BlueprintPure)
	float GetPatrolDistance() const;

	UFUNCTION(BlueprintPure)
	float GetPatrolDuration() const;

	UFUNCTION(BlueprintPure)
	FVector GetAIMoveLocation() const { return AIMoveLocation; }
		
	template<class T>
	T* GetEquip(EDWEquipPart InEquipPart) const
	{
		return Cast<T>(GetEquip(InEquipPart));
	}

	UFUNCTION(BlueprintPure)
	ADWEquip* GetEquip(EDWEquipPart InEquipPart) const;
					
	UFUNCTION(BlueprintPure)
	TArray<ADWEquip*> GetEquips() const;

	template<class T>
	T* GetWeapon(EDWWeaponPart InWeaponPart) const
	{
		return Cast<T>(GetWeapon(InWeaponPart));
	}

	UFUNCTION(BlueprintPure)
	ADWEquipWeapon* GetWeapon(EDWWeaponPart InWeaponPart) const;
	
	UFUNCTION(BlueprintPure)
	TArray<ADWEquipWeapon*> GetWeapons() const;

	UFUNCTION(BlueprintPure)
	EDWWeaponType GetWeaponType(EDWWeaponPart InWeaponPart) const;

	UFUNCTION(BlueprintPure)
	bool CheckWeaponType(EDWWeaponPart InWeaponPart, EDWWeaponType InWeaponType) const;

	UFUNCTION(BlueprintPure)
	TSubclassOf<AAbilityProjectileBase> GetWeaponProjectile(EDWWeaponPart InWeaponPart) const;

	UFUNCTION(BlueprintPure)
	EDWCharacterAttackType GetAttackType() const { return AttackType; }

	UFUNCTION(BlueprintPure)
	bool HasAttackAbility(EDWWeaponType InWeaponType, int32 InAbilityIndex = 0) const;

	UFUNCTION(BlueprintPure)
	FDWCharacterAttackAbilityData GetAttackAbility(EDWWeaponType InWeaponType, int32 InAbilityIndex = -1) const;

	UFUNCTION(BlueprintPure)
	FDWCharacterAttackAbilityQueue GetAttackAbilityQueue(EDWWeaponType InWeaponType) const;

	FDWCharacterAttackAbilityQueue& GetAttackAbilityQueue(EDWWeaponType InWeaponType);

	UFUNCTION(BlueprintPure)
	int32 GetAttackAbilityIndex(EDWWeaponType InWeaponType) const;

	UFUNCTION(BlueprintPure)
	TArray<FDWCharacterAttackAbilityData> GetAttackAbilities(EDWWeaponType InWeaponType) const;

	UFUNCTION(BlueprintPure)
	TMap<EDWWeaponType, FDWCharacterAttackAbilityQueue>& GetAttackAbilityQueues() { return AttackAbilityQueues; }

	UFUNCTION(BlueprintPure)
	bool HasFallingAttackAbility(EDWWeaponType InWeaponType) const;

	UFUNCTION(BlueprintPure)
	FDWCharacterFallingAttackAbilityData GetFallingAttackAbility(EDWWeaponType InWeaponType);

	UFUNCTION(BlueprintPure)
	TMap<EDWWeaponType, FDWCharacterFallingAttackAbilityData>& GetFallingAttackAbilities() { return FallingAttackAbilities; }

	UFUNCTION(BlueprintPure)
	bool HasSkillAttackAbility(const FPrimaryAssetId& InSkillID, bool bNeedSlotted = false) const;

	UFUNCTION(BlueprintPure)
	FDWCharacterSkillAttackAbilityData GetSkillAttackAbility(const FPrimaryAssetId& InSkillID, bool bNeedSlotted = false);
	
	UFUNCTION(BlueprintPure)
	FAbilityItem& GetSkillAttackAbilityItem() { return SkillAttackAbilityItem; }

	UFUNCTION(BlueprintPure)
	TMap<FPrimaryAssetId, FDWCharacterSkillAttackAbilityData>& GetSkillAttackAbilities() { return SkillAttackAbilities; }

	UFUNCTION(BlueprintPure)
	UDWCharacterPart* GetCharacterPart(EDWCharacterPart InCharacterPart) const;

	virtual UBehaviorTree* GetBehaviorTreeAsset() const override;
};
