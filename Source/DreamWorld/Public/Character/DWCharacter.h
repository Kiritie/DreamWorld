// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Ability/Character/AbilityCharacterBase.h"
#include "Ability/Character/DWCharacterAttributeSet.h"
#include "Common/DWCommonTypes.h"
#include "Team/Agent/DWTeamAgentInterface.h"

#include "DWCharacter.generated.h"

class UWorldWidgetComponent;
class UDWCharacterData;
class ADWVoxelChunk;
class UInteractionComponent;
class UInteractionComponent;
class UDWCharacterPart;
class ADWEquipArmor;
class ADWEquipShield;
class ADWEquipWeapon;
class UAbilityInventoryEquipSlot;
class AVoxelChunk;
class UVoxel;
class AController;
class UDWCharacterAnim;
class UAbilityBase;
class UAbilitySystemComponentBase;
class UAbilityCharacterInventoryBase;
class UBehaviorTree;
class UDWAIBlackboard;
class UAbilityInventorySlot;
class AAbilitySkillBase;

/**
 * 角色
 */
UCLASS()
class DREAMWORLD_API ADWCharacter : public AAbilityCharacterBase, public IDWTeamAgentInterface
{
	GENERATED_UCLASS_BODY()

	friend class ADWPlayerController;
	
	friend class UDWCharacterState_Attack;
	friend class UDWCharacterState_Climb;
	friend class UDWCharacterState_Crouch;
	friend class UDWCharacterState_Death;
	friend class UDWCharacterState_Default;
	friend class UDWCharacterState_Defend;
	friend class UDWCharacterState_Dodge;
	friend class UDWCharacterState_Fall;
	friend class UDWCharacterState_Float;
	friend class UDWCharacterState_Fly;
	friend class UDWCharacterState_Interrupt;
	friend class UDWCharacterState_Jump;
	friend class UDWCharacterState_Ride;
	friend class UDWCharacterState_Static;
	friend class UDWCharacterState_Swim;
	friend class UDWCharacterState_Walk;

public:
	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

protected:
	virtual int32 GetLimit_Implementation() const override { return 0; }

	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;
	
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void ResetData() override;

	virtual void SetActorVisible_Implementation(bool bInVisible) override;

	virtual void OnFiniteStateRefresh(UFiniteStateBase* InCurrentState) override;
	
public:
	virtual void Serialize(FArchive& Ar) override;

public:
	virtual void Death(IAbilityVitalityInterface* InKiller = nullptr) override;
			
	virtual void Revive(IAbilityVitalityInterface* InRescuer = nullptr) override;

	virtual bool CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassivity) override;

	virtual void OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess) override;
		
	virtual void OnCancelItem(const FAbilityItem& InItem, bool bPassive) override;

	virtual void OnAssembleItem(const FAbilityItem& InItem) override;

	virtual void OnDischargeItem(const FAbilityItem& InItem) override;

	virtual void OnDiscardItem(const FAbilityItem& InItem, bool bInPlace) override;

	virtual void OnSelectItem(const FAbilityItem& InItem) override;

	virtual void OnAuxiliaryItem(const FAbilityItem& InItem) override;

	UFUNCTION(BlueprintCallable)
	virtual void FreeToAnim(bool bUnLockRotation = true);

	UFUNCTION(BlueprintCallable)
	virtual void LimitToAnim(bool bLockRotation = false);

	UFUNCTION(BlueprintCallable)
	virtual void Interrupt(float InDuration = -1);

	UFUNCTION(BlueprintCallable)
	virtual void UnInterrupt();

	virtual void Jump() override;

	virtual void UnJump() override;

	UFUNCTION(BlueprintCallable)
	virtual void Dodge();

	UFUNCTION(BlueprintCallable)
	virtual void UnDodge();

	UFUNCTION(BlueprintCallable)
	virtual void Sprint();

	UFUNCTION(BlueprintCallable)
	virtual void UnSprint();

	virtual void Crouch(bool bClientSimulation) override;

	virtual void UnCrouch(bool bClientSimulation) override;

	UFUNCTION(BlueprintCallable)
	virtual void Swim();
		
	UFUNCTION(BlueprintCallable)
	virtual void UnSwim();
						
	UFUNCTION(BlueprintCallable)
	virtual void Float(float InWaterPosZ);
						
	UFUNCTION(BlueprintCallable)
	virtual void UnFloat();

	UFUNCTION(BlueprintCallable)
	virtual void Climb();
			
	UFUNCTION(BlueprintCallable)
	virtual void UnClimb();

	UFUNCTION(BlueprintCallable)
	virtual void Ride(ADWCharacter* InTarget);

	UFUNCTION(BlueprintCallable)
	virtual void UnRide();

	UFUNCTION(BlueprintCallable)
	virtual void Fly();

	UFUNCTION(BlueprintCallable)
	virtual void UnFly();

	virtual bool Attack(int32 InAbilityIndex = -1, const FSimpleDelegate& OnStart = nullptr, const FSimpleDelegate& OnEnd = nullptr);

	virtual bool SkillAttack(const FPrimaryAssetId& InSkillID, const FSimpleDelegate& OnStart = nullptr, const FSimpleDelegate& OnEnd = nullptr);

	virtual bool SkillAttack(ESkillType InSkillType, int32 InAbilityIndex = -1, const FSimpleDelegate& OnStart = nullptr, const FSimpleDelegate& OnEnd = nullptr);
	
	virtual bool SkillAttackImpl(const FPrimaryAssetId& InSkillID, const FSimpleDelegate& OnStart = nullptr, const FSimpleDelegate& OnEnd = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void UnAttack();

	UFUNCTION(BlueprintCallable)
	virtual void Defend();
	
	UFUNCTION(BlueprintCallable)
	virtual void UnDefend();

	virtual bool OnPickUp(AAbilityPickUpBase* InPickUp) override;

	virtual bool OnGenerateVoxel(const FVoxelHitResult& InVoxelHitResult) override;

	virtual bool OnDestroyVoxel(const FVoxelHitResult& InVoxelHitResult) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual bool DoAction(EDWCharacterActionType InActionType);

	UFUNCTION(BlueprintCallable)
	virtual bool StopAction(EDWCharacterActionType InActionType);

	UFUNCTION(BlueprintCallable)
	virtual void EndAction(EDWCharacterActionType InActionType, bool bWasCancelled);

	UFUNCTION(BlueprintCallable)
	virtual void AIMoveTo(FVector InTargetLocation, float InMoveStopDistance = 10.f, bool bMulticast = false) override;

	UFUNCTION(BlueprintCallable)
	virtual bool DoAIMove(FVector InTargetLocation, float InMoveStopDistance = 10.f);
	
	virtual bool DoAIMove(ADWCharacter* InTargetCharacter, float InMoveStopDistance = 10.f, bool bLookAtTarget = false);

	virtual void StopAIMove(bool bMulticast = false) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
									
	virtual void SetMotionRate_Implementation(float InMovementRate, float InRotationRate) override;
				
	virtual bool IsAttackHitAble() const;

	virtual void SetAttackHitAble(bool bValue);
		
	virtual void ClearAttackHitTargets();

	virtual bool RaycastStep(FHitResult& OutHitResult);
	
	virtual bool IsTargetAble_Implementation(APawn* InPlayerPawn) const override;

	virtual bool IsLookAtAble_Implementation(AActor* InLookerActor) const override;

	virtual bool CanLookAtTarget() override;

public:
	UFUNCTION(BlueprintPure)
	bool HasAttackAbility(int32 InAbilityIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	bool HasSkillAbility(const FPrimaryAssetId& InSkillID, bool bNeedAssembled = false) const;

	bool HasSkillAbility(ESkillType InSkillType, int32 InAbilityIndex = 0, bool bNeedAssembled = false) const;
	
	UFUNCTION(BlueprintCallable)
	bool HasActionAbility(EDWCharacterActionType InActionType) const;

public:
	virtual bool IsEnemy(IAbilityPawnInterface* InTarget) const override;
	
	UFUNCTION(BlueprintPure)
	virtual bool IsExhausted() const;
	
	UFUNCTION(BlueprintPure)
	virtual bool IsFreeToAnim() const;
	
	UFUNCTION(BlueprintPure)
	virtual bool IsDodging() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsSprinting() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsCrouching(bool bReally = false) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsSwimming(bool bReally = false) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsFloating() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsAttacking(bool bCheckAttackType = false) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsDefending() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsClimbing() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsRiding() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsFlying(bool bReally = false) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsInterrupting() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsLockRotation() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsBreakAllInput() const;
	
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
	ADWCharacter* OwnerRider;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStats")
	ADWCharacter* RidingTarget;

protected:
	FVector BirthLocation;

	bool bAttackHitAble;
	
	float AIMoveStopDistance;

	FVector AIMoveLocation;
	
	float CameraDoRotationTime;
	float CameraDoRotationDuration;
	FRotator CameraDoRotationRotation;

	int32 AttackAbilityIndex;
		
	int32 AttackAbilityQueue;

	FPrimaryAssetId SkillAbilityID;

	FTimerHandle AttackHurtTimer;

	EDWCharacterAttackType AttackType;

	UPROPERTY()
	TMap<EDWEquipPartType, AAbilityEquipBase*> Equips;
	
	FDWCharacterAttackAbilityData FallingAttackAbility;

	TMap<EDWWeaponType, FDWCharacterAttackAbilityDatas> AttackAbilities;

	TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData> SkillAbilities;

	TMap<EDWCharacterActionType, FDWCharacterActionAbilityData> ActionAbilities;

public:
	virtual FGuid GetActorIDT() const override { return ActorID; }
	
	virtual FName GetNameT() const override { return Name; }
	
	virtual void SetNameV(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual bool SetLevelA(int32 InLevel) override;

	UFUNCTION(BlueprintNativeEvent)
	void SetControlMode(EDWCharacterControlMode InControlMode);

	UFUNCTION(BlueprintPure)
	EDWCharacterControlMode GetControlMode() const { return ControlMode; }

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
	ADWCharacter* GetOwnerRider() const { return OwnerRider; }

	UFUNCTION(BlueprintPure)
	ADWCharacter* GetRidingTarget() const { return RidingTarget; }

	UFUNCTION(BlueprintPure)
	FVector GetBirthLocation() const { return BirthLocation; }

	UFUNCTION(BlueprintPure)
	FVector GetAIMoveLocation() const { return AIMoveLocation; }

	UFUNCTION(BlueprintPure)
	int32 GetAttackAbilityIndex() const { return AttackAbilityIndex; }

	UFUNCTION(BlueprintPure)
	FPrimaryAssetId GetSkillAbilityID() const { return SkillAbilityID; }

	UFUNCTION(BlueprintPure)
	EDWCharacterAttackType GetAttackType() const { return AttackType; }
		
	template<class T>
	T* GetEquip(EDWEquipPartType InPartType) const
	{
		return Cast<T>(GetEquip(InPartType));
	}

	UFUNCTION(BlueprintPure)
	AAbilityEquipBase* GetEquip(EDWEquipPartType InPartType) const;
			
	UFUNCTION(BlueprintPure)
	ADWEquipWeapon* GetWeapon() const;
				
	UFUNCTION(BlueprintPure)
	ADWEquipShield* GetShield() const;

	UFUNCTION(BlueprintPure)
	EDWWeaponType GetWeaponType() const;
				
	UFUNCTION(BlueprintPure)
	bool CheckWeaponType(EDWWeaponType InWeaponType) const;

	UFUNCTION(BlueprintPure)
	EDWShieldType GetShieldType() const;
				
	UFUNCTION(BlueprintPure)
	bool CheckShieldType(EDWShieldType InShieldType) const;

	UFUNCTION(BlueprintPure)
	TMap<EDWEquipPartType, AAbilityEquipBase*> GetEquips() const { return Equips; }

	UFUNCTION(BlueprintPure)
	FDWCharacterAttackAbilityData GetAttackAbility(int32 InAbilityIndex = -1) const;
		
	UFUNCTION(BlueprintPure)
	FDWCharacterSkillAbilityData GetSkillAbility(const FPrimaryAssetId& InSkillID, bool bNeedAssembled = false);
		
	FDWCharacterSkillAbilityData GetSkillAbility(ESkillType InSkillType, int32 InAbilityIndex = -1, bool bNeedAssembled = false);
			
	UFUNCTION(BlueprintPure)
	FDWCharacterActionAbilityData GetActionAbility(EDWCharacterActionType InActionType);

	UFUNCTION(BlueprintPure)
	TArray<FDWCharacterAttackAbilityData> GetAttackAbilities() const;

	UFUNCTION(BlueprintPure)
	TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData> GetSkillAbilities() const { return SkillAbilities; }

	UFUNCTION(BlueprintPure)
	TMap<EDWCharacterActionType, FDWCharacterActionAbilityData> GetActionAbilities() const { return ActionAbilities; }

	UFUNCTION(BlueprintPure)
	UDWCharacterPart* GetCharacterPart(EDWCharacterPartType InCharacterPartType) const;

	virtual UBehaviorTree* GetBehaviorTreeAsset() const override;

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Mana)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MaxMana)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Stamina)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MaxStamina)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, SwimSpeed)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, RideSpeed)
		
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, FlySpeed)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DodgeForce)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackForce)
		
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, RepulseForce)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackSpeed)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackCritRate)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackStealRate)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DefendRate)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DefendScope)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, PhysicsDefRate)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MagicDefRate)
	
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ToughnessRate)
			
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaRegenSpeed)
			
	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaExpendSpeed)

	ATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Interrupt)

public:
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;
	
	virtual void HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, bool bHasDefend, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;
	
	virtual void HandleInterrupt(float InterruptDuration);
};
