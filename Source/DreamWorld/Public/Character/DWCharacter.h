// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TargetSystemComponent.h"
#include "TargetSystemTargetableInterface.h"
#include "Ability/Character/AbilityCharacterBase.h"
#include "Ability/Character/DWCharacterAttributeSet.h"
#include "Inventory/InventoryAgentInterface.h"
#include "Team/DWTeamModuleTypes.h"
#include "Voxel/Agent/VoxelAgentInterface.h"

#include "DWCharacter.generated.h"

class UWorldWidgetComponent;
class UDWCharacterData;
class ADWVoxelChunk;
class UCharacterInteractionComponent;
class UInteractionComponent;
class UDWCharacterPart;
class ADWEquipArmor;
class ADWEquipShield;
class ADWEquipWeapon;
class UInventoryEquipSlot;
class AVoxelChunk;
class UVoxel;
class AController;
class UDWCharacterAnim;
class UAbilityBase;
class UAbilitySystemComponentBase;
class UCharacterInventory;
class UBehaviorTree;
class UDWAIBlackboard;
class UInventorySlot;
class AAbilitySkillBase;

/**
 * 角色
 */
UCLASS()
class DREAMWORLD_API ADWCharacter : public AAbilityCharacterBase, public ITargetSystemTargetableInterface, public IInventoryAgentInterface
{
	GENERATED_BODY()

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
	ADWCharacter();
	
protected:
	// stats
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStates")
	EDWCharacterControlMode ControlMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	FName TeamID;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	FVector BirthLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	ADWCharacter* OwnerRider;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	ADWCharacter* RidingTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	ADWCharacter* LockedTarget;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWorldWidgetComponent* CharacterHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterInventory* Inventory;

protected:
	float DefaultGravityScale;

	float DefaultAirControl;
	
	float AIMoveStopDistance;

	FVector AIMoveLocation;

	int32 AttackAbilityIndex;

	FPrimaryAssetId SkillAbilityID;

	FTimerHandle AttackHurtTimer;

	EDWCharacterAttackType AttackType;

	UPROPERTY()
	TMap<EDWEquipPartType, AAbilityEquipBase*> Equips;
	
	FDWCharacterAttackAbilityData FallingAttackAbility;

	TMap<EDWWeaponType, FDWCharacterAttackAbilityDatas> AttackAbilities;

	TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData> SkillAbilities;

	TMap<EDWCharacterActionType, FDWCharacterActionAbilityData> ActionAbilities;

protected:
	virtual void BeginPlay() override;

	virtual int32 GetLimit_Implementation() const override { return 0; }

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation() override;
	
	virtual void LoadData(FSaveData* InSaveData, bool bForceMode) override;

	virtual FSaveData* ToData() override;

	virtual void SetActorVisible_Implementation(bool bNewVisible) override;

	virtual void RefreshState() override;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void Serialize(FArchive& Ar) override;

public:
	virtual void Death(IAbilityVitalityInterface* InKiller = nullptr) override;
			
	virtual void Revive(IAbilityVitalityInterface* InRescuer = nullptr) override;

	virtual bool CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual void OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	UFUNCTION(BlueprintCallable)
	virtual void FreeToAnim(bool bUnLockRotation = true);

	UFUNCTION(BlueprintCallable)
	virtual void LimitToAnim(bool bLockRotation = false, bool bUnSprint = false);

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

	UFUNCTION(BlueprintCallable)
	virtual bool Attack(int32 InAbilityIndex = -1);

	UFUNCTION(BlueprintCallable)
	virtual bool SkillAttack(const FPrimaryAssetId& InSkillID);

	virtual bool SkillAttack(ESkillType InSkillType, int32 InAbilityIndex = -1);
		
	UFUNCTION(BlueprintCallable)
	virtual bool FallingAttack();

	UFUNCTION(BlueprintCallable)
	virtual void UnAttack();

	UFUNCTION(BlueprintCallable)
	virtual void Defend();
	
	UFUNCTION(BlueprintCallable)
	virtual void UnDefend();

	virtual bool UseItem(FAbilityItem& InItem);

	virtual void PickUp(AAbilityPickUpBase* InPickUp) override;

	virtual bool GenerateVoxel(const FVoxelHitResult& InVoxelHitResult) override;

	virtual bool DestroyVoxel(const FVoxelHitResult& InVoxelHitResult) override;

	UFUNCTION(BlueprintCallable)
	virtual void RefreshEquip(EDWEquipPartType InPartType, const FAbilityItem& InItem);

public:
	UFUNCTION(BlueprintCallable)
	virtual bool DoAction(EDWCharacterActionType InActionType);

	UFUNCTION(BlueprintCallable)
	virtual bool StopAction(EDWCharacterActionType InActionType);

	UFUNCTION(BlueprintCallable)
	virtual void EndAction(EDWCharacterActionType InActionType, bool bWasCancelled);

	UFUNCTION(BlueprintCallable)
	virtual void SetLockedTarget(ADWCharacter* InTargetCharacter);
		
	UFUNCTION(BlueprintCallable)
	virtual void LookAtTarget(ADWCharacter* InTargetCharacter);

	UFUNCTION(BlueprintCallable)
	virtual void AIMoveTo(FVector InTargetLocation, float InMoveStopDistance = 10.f, bool bMulticast = false) override;

	UFUNCTION(BlueprintCallable)
	virtual bool DoAIMove(FVector InTargetLocation, float InMoveStopDistance = 10.f);
	
	virtual bool DoAIMove(ADWCharacter* InTargetCharacter, float InMoveStopDistance = 10.f, bool bLookAtTarget = false);

	virtual void StopAIMove(bool bMulticast = false) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	virtual void SetAttackDamageAble(bool bInDamaging);

	virtual bool RaycastStep(FHitResult& OutHitResult);

public:
	UFUNCTION(BlueprintPure)
	bool HasTeam() const;

	UFUNCTION(BlueprintPure)
	bool IsTeamMate(ADWCharacter* InTargetCharacter) const;

	UFUNCTION(BlueprintPure)
	bool HasAttackAbility(int32 InAbilityIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	bool HasSkillAbility(const FPrimaryAssetId& InSkillID, bool bNeedAssembled = false) const;

	bool HasSkillAbility(ESkillType InSkillType, int32 InAbilityIndex = 0, bool bNeedAssembled = false) const;
	
	UFUNCTION(BlueprintCallable)
	bool HasActionAbility(EDWCharacterActionType InActionType) const;

	UFUNCTION(BlueprintCallable)
	bool CreateTeam(const FName& InTeamName = NAME_None, FString InTeamDetail = TEXT(""));
		
	UFUNCTION(BlueprintCallable)
	bool DissolveTeam();

	UFUNCTION(BlueprintCallable)
	bool JoinTeam(const FName& InTeamID);
		
	bool JoinTeam(ADWCharacter* InTargetCharacter);

	UFUNCTION(BlueprintCallable)
	bool LeaveTeam();
		
	UFUNCTION(BlueprintCallable)
	bool AddTeamMate(ADWCharacter* InTargetCharacter);
	
	UFUNCTION(BlueprintCallable)
	bool RemoveTeamMate(ADWCharacter* InTargetCharacter);

	UFUNCTION(BlueprintCallable)
	TArray<ADWCharacter*> GetTeamMates();

	UFUNCTION(BlueprintPure)
	virtual bool IsPlayer() const;
	
	UFUNCTION(BlueprintPure)
	virtual bool IsEnemy(ADWCharacter* InTargetCharacter) const;
									
	virtual void SetMotionRate_Implementation(float InMovementRate, float InRotationRate) override;

public:
	UFUNCTION(BlueprintPure)
	UWorldWidgetComponent* GetCharacterHP() const { return CharacterHP; }

	UFUNCTION(BlueprintPure)
	UWidgetCharacterHP* GetCharacterHPWidget() const;

	UFUNCTION(BlueprintPure)
	virtual UInventory* GetInventory() const override;

public:
	UFUNCTION(BlueprintPure)
	virtual bool IsExhausted() const;
	
	UFUNCTION(BlueprintPure)
	virtual bool IsFreeToAnim() const;
	
	UFUNCTION(BlueprintPure)
	virtual bool IsDodging() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsSprinting() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsCrouching(bool bMovementMode = false) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsSwimming(bool bMovementMode = false) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsFloating() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsAttacking(bool bAttackType = false) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsDefending() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsClimbing() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsRiding() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsFlying(bool bMovementMode = false) const;

	UFUNCTION(BlueprintPure)
	virtual bool IsInterrupting() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsLockRotation() const;

	UFUNCTION(BlueprintPure)
	virtual bool IsBreakAllInput() const;
	
	virtual bool IsTargetable_Implementation() const override;

public:
	virtual void SetNameV(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual void SetLevelV(int32 InLevel) override;

	UFUNCTION(BlueprintPure)
	EDWCharacterControlMode GetControlMode() const { return ControlMode; }
		
	UFUNCTION(BlueprintCallable)
	virtual void SetControlMode(EDWCharacterControlMode InControlMode);

	FDWTeamData* GetTeamData() const;

	UFUNCTION(BlueprintPure)
	FName GetTeamID() const { return TeamID; }

	UFUNCTION(BlueprintCallable)
	virtual void SetTeamID(FName InTeamID);
	
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
	ADWCharacter* GetLockedTarget() const { return LockedTarget; }

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
	FDWCharacterSkillAbilityData GetSkillAbility(const FPrimaryAssetId& InSkillID);
		
	FDWCharacterSkillAbilityData GetSkillAbility(ESkillType InSkillType, int32 InAbilityIndex = -1);
			
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
	virtual void OnSelectedItemChange(const FAbilityItem& InItem) override;
	
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;
	
	virtual void HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, bool bHasDefend, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;
	
	virtual void HandleInterrupt(float InterruptDuration);
};
