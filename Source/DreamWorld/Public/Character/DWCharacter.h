// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TargetSystemComponent.h"
#include "TargetSystemTargetableInterface.h"
#include "Ability/Character/AbilityCharacterBase.h"
#include "Inventory/InventoryAgentInterface.h"
#include "Scene/Actor/PickUp/PickerInterface.h"
#include "Voxel/Agent/VoxelAgentInterface.h"

#include "DWCharacter.generated.h"

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
class UWidgetCharacterHPComponent;
class UAbilityBase;
class UAbilitySystemComponentBase;
class UAIPerceptionStimuliSourceComponent;
class UCharacterInventory;
class UBehaviorTree;
class UDWAIBlackboard;
class UInventorySlot;
class AAbilitySkillBase;

/**
 * 角色
 */
UCLASS()
class DREAMWORLD_API ADWCharacter : public AAbilityCharacterBase, public IVoxelAgentInterface, public ITargetSystemTargetableInterface, public IPickerInterface, public IInventoryAgentInterface
{
	GENERATED_BODY()

public:
	ADWCharacter();
	
protected:
	// states
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bDying;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bActive;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bDodging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bSprinting;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bCrouching;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bSwimming;
				
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bFloating;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bClimbing;
					
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bRiding;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bFlying;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bAttacking;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bDefending;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bDamaging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bInterrupting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bLockRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bFreeToAnimate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bBreakAllInput;

protected:
	// stats
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	EDWCharacterNature Nature;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	FName TeamID;
			
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	float AttackDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	float InteractDistance;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	float FollowDistance;
			
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	float PatrolDistance;
			
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	float PatrolDuration;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	FVector BirthLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	AVoxelChunk* OwnerChunk;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	ADWCharacter* OwnerRider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	ADWCharacter* RidingTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStats")
	ADWCharacter* LockedTarget;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionStimuliSourceComponent* StimuliSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetCharacterHPComponent* WidgetCharacterHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterInventory* Inventory;

protected:
	float DefaultGravityScale;

	float DefaultAirControl;
	
	float AIMoveStopDistance;

	float InterruptRemainTime;

	float NormalAttackRemainTime;

	FVector AIMoveLocation;

	int32 AttackAbilityIndex;

	FPrimaryAssetId SkillAbilityID;

	EDWAttackType AttackType;

	EDWCharacterActionType ActionType;

	FTimerHandle AttackHurtTimer;

	FVoxelItem SelectedVoxelItem;

	UPROPERTY()
	TMap<EDWEquipPartType, AAbilityEquipBase*> Equips;
	
	FDWCharacterAttackAbilityData FallingAttackAbility;

	TArray<FDWCharacterAttackAbilityData> AttackAbilities;

	TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData> SkillAbilities;

	TMap<EDWCharacterActionType, FDWCharacterActionAbilityData> ActionAbilities;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Serialize(FArchive& Ar) override;

	virtual void LoadData(FSaveData* InSaveData) override;

	virtual FSaveData* ToData(bool bSaved = true) override;

	virtual void ResetData(bool bRefresh = false) override;

	virtual void RefreshData() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Active(bool bResetData = false);
	
	UFUNCTION(BlueprintCallable)
	virtual void Disable(bool bDisableMovement = false, bool bDisableCollision = false);
						
	virtual void Spawn() override;
		
	virtual void Revive() override;
			
	virtual void Death(AActor* InKiller = nullptr) override;
			
	virtual void DeathStart();

	UFUNCTION(BlueprintCallable)
	virtual void DeathEnd();

	virtual bool CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual void OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	UFUNCTION(BlueprintCallable)
	virtual void FreeToAnim(bool bUnLockRotation = true);

	UFUNCTION(BlueprintCallable)
	virtual void LimitToAnim(bool bInLockRotation = false, bool bUnSprint = false);

	UFUNCTION(BlueprintCallable)
	virtual void Interrupt(float InDuration = -1, bool bDoAction = false);
			
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

	virtual void Crouch(bool bClientSimulation = false) override;

	virtual void UnCrouch(bool bClientSimulation = false) override;
		
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
	virtual void AttackStart();

	UFUNCTION(BlueprintCallable)
	virtual void AttackHurt();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void UnAttack();

	UFUNCTION(BlueprintCallable)
	virtual void Defend();

	virtual void UnDefend();

	virtual void FallStart();

	virtual void FallEnd();
	
	virtual bool UseItem(FAbilityItem& InItem);

	virtual void PickUp(APickUp* InPickUp) override;

	UFUNCTION(BlueprintCallable)
	virtual bool GenerateVoxel(const FVoxelHitResult& InVoxelHitResult, FAbilityItem& InItem) override;

	UFUNCTION(BlueprintCallable)
	virtual bool DestroyVoxel(const FVoxelHitResult& InVoxelHitResult) override;

	UFUNCTION(BlueprintCallable)
	virtual void RefreshEquip(EDWEquipPartType InPartType, UInventoryEquipSlot* EquipSlot);

public:
	virtual bool GetAbilityInfo(TSubclassOf<UAbilityBase> AbilityClass, FAbilityInfo& OutAbilityInfo) override;

	UFUNCTION(BlueprintCallable)
	virtual bool DoAction(EDWCharacterActionType InActionType);

	UFUNCTION(BlueprintCallable)
	virtual void EndAction(EDWCharacterActionType InActionType);

	//UFUNCTION(BlueprintCallable)
	virtual bool StopAction(EDWCharacterActionType InActionType = EDWCharacterActionType::None, bool bCancelAbility = true, bool bEndAction = false);
					
	UFUNCTION(BlueprintCallable)
	virtual void ModifyMana(float InDeltaValue);
									
	UFUNCTION(BlueprintCallable)
	virtual void ModifyStamina(float InDeltaValue);

	UFUNCTION(BlueprintCallable)
	virtual void SetLockedTarget(ADWCharacter* InTargetCharacter);
		
	UFUNCTION(BlueprintCallable)
	virtual void LookAtTarget(ADWCharacter* InTargetCharacter);

	UFUNCTION(BlueprintCallable)
	virtual void AIMoveTo(FVector InTargetLocation, float InMoveStopDistance = 10.f, bool bMulticast = false);

	UFUNCTION(BlueprintCallable)
	virtual bool DoAIMove(FVector InTargetLocation, float InMoveStopDistance = 10.f);
	
	virtual bool DoAIMove(ADWCharacter* InTargetCharacter, float InMoveStopDistance = 10.f, bool bLookAtTarget = false);

	virtual void StopAIMove(bool bMulticast = false) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	virtual void SetDamaging(bool bInDamaging);

	virtual bool RaycastStep(FHitResult& OutHitResult);

	virtual bool RaycastVoxel(FVoxelHitResult& OutHitResult);

public:
	UFUNCTION(BlueprintPure)
	bool HasTeam() const;

	UFUNCTION(BlueprintPure)
	bool IsTeamMate(ADWCharacter* InTargetCharacter) const;

	UFUNCTION(BlueprintPure)
	bool HasAttackAbility(int32 InAbilityIndex = -1) const;

	UFUNCTION(BlueprintCallable)
	bool HasSkillAbility(const FPrimaryAssetId& InSkillID);

	bool HasSkillAbility(ESkillType InSkillType, int32 InAbilityIndex = -1);
	
	UFUNCTION(BlueprintCallable)
	bool HasActionAbility(EDWCharacterActionType InActionType);

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

	virtual void SetVisible_Implementation(bool bVisible) override;
									
	virtual void SetMotionRate_Implementation(float InMovementRate, float InRotationRate) override;

public:
	UFUNCTION(BlueprintPure)
	UAIPerceptionStimuliSourceComponent* GetStimuliSource() const { return StimuliSource; }
	
	UFUNCTION(BlueprintPure)
	UWidgetCharacterHPComponent* GetWidgetCharacterHP() const { return WidgetCharacterHP; }

	UFUNCTION(BlueprintPure)
	virtual UInventory* GetInventory() const override;

	UFUNCTION(BlueprintPure)
	bool HasBehaviorTree() const;

	UFUNCTION(BlueprintPure)
	UBehaviorTree* GetBehaviorTree();
	
	UFUNCTION(BlueprintPure)
	UWidgetCharacterHP* GetWidgetCharacterHPWidget() const;

public:
	UFUNCTION(BlueprintPure)
	bool IsActive(bool bNeedFreeToAnim = false) const;
																	
	UFUNCTION(BlueprintPure)
	bool IsFreeToAnim(bool bCheckStates = true) const;
	
	virtual bool IsDead() const override;

	UFUNCTION(BlueprintPure)
	bool IsDying() const { return bDying; }

	UFUNCTION(BlueprintPure)
	bool IsFalling() const { return bFalling; }
	
	UFUNCTION(BlueprintPure)
	bool IsDodging() const { return bDodging; }

	UFUNCTION(BlueprintPure)
	bool IsSprinting() const { return bSprinting; }

	UFUNCTION(BlueprintPure)
	bool IsCrouching() const { return bCrouching; }

	UFUNCTION(BlueprintPure)
	bool IsSwimming() const { return bSwimming; }

	UFUNCTION(BlueprintPure)
	bool IsFloating() const { return bFloating; }

	UFUNCTION(BlueprintPure)
	bool IsAttacking() const { return bAttacking; }

	UFUNCTION(BlueprintPure)
	bool IsDefending() const { return bDefending; }

	UFUNCTION(BlueprintPure)
	bool IsDamaging() const { return bDamaging; }

	UFUNCTION(BlueprintPure)
	bool IsClimbing() const { return bClimbing; }

	UFUNCTION(BlueprintPure)
	bool IsRiding() const { return bRiding; }

	UFUNCTION(BlueprintPure)
	bool IsFlying() const { return bFlying; }

	UFUNCTION(BlueprintPure)
	bool IsInterrupting() const { return bInterrupting; }

	UFUNCTION(BlueprintPure)
	bool IsLockRotation() const { return bLockRotation; }

	UFUNCTION(BlueprintPure)
	bool IsBreakAllInput() const { return bBreakAllInput; }
	
	virtual bool IsTargetable_Implementation() const override;

public:
	UFUNCTION(BlueprintPure)
	EDWCharacterNature GetNature() const { return Nature; }

	//UFUNCTION(BlueprintPure)
	FDWTeamData* GetTeamData() const;

	UFUNCTION(BlueprintPure)
	FName GetTeamID() const { return TeamID; }

	UFUNCTION(BlueprintCallable)
	void SetTeamID(FName InTeamID);
			
	UFUNCTION(BlueprintPure)
	float GetMana() const;
			
	UFUNCTION(BlueprintCallable)
	void SetMana(float InValue);

	UFUNCTION(BlueprintPure)
	float GetMaxMana() const;
			
	UFUNCTION(BlueprintCallable)
	void SetMaxMana(float InValue);
		
	UFUNCTION(BlueprintPure)
	float GetStamina() const;
		
	UFUNCTION(BlueprintCallable)
	void SetStamina(float InValue);

	UFUNCTION(BlueprintPure)
	float GetMaxStamina() const;
		
	UFUNCTION(BlueprintCallable)
	void SetMaxStamina(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetSwimSpeed() const;
		
	UFUNCTION(BlueprintCallable)
	void SetSwimSpeed(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetRideSpeed() const;
		
	UFUNCTION(BlueprintCallable)
	void SetRideSpeed(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetFlySpeed() const;
		
	UFUNCTION(BlueprintCallable)
	void SetFlySpeed(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetDodgeForce() const;
		
	UFUNCTION(BlueprintCallable)
	void SetDodgeForce(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetAttackForce() const;
		
	UFUNCTION(BlueprintCallable)
	void SetAttackForce(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetRepulseForce() const;
		
	UFUNCTION(BlueprintCallable)
	void SetRepulseForce(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetAttackSpeed() const;
		
	UFUNCTION(BlueprintCallable)
	void SetAttackSpeed(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetAttackCritRate() const;
		
	UFUNCTION(BlueprintCallable)
	void SetAttackCritRate(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetAttackStealRate() const;
		
	UFUNCTION(BlueprintCallable)
	void SetAttackStealRate(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetDefendRate() const;
		
	UFUNCTION(BlueprintCallable)
	void SetDefendRate(float InValue);

	UFUNCTION(BlueprintPure)
	float GetDefendScope() const;
		
	UFUNCTION(BlueprintCallable)
	void SetDefendScope(float InValue);

	UFUNCTION(BlueprintPure)
	float GetPhysicsDefRate() const;
		
	UFUNCTION(BlueprintCallable)
	void SetPhysicsDefRate(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetMagicDefRate() const;
		
	UFUNCTION(BlueprintCallable)
	void SetMagicDefRate(float InValue);
			
	UFUNCTION(BlueprintPure)
	float GetToughnessRate() const;
				
	UFUNCTION(BlueprintCallable)
	void SetToughnessRate(float InValue);
	
	UFUNCTION(BlueprintPure)
	float GetStaminaRegenSpeed() const;
			
	UFUNCTION(BlueprintCallable)
	void SetStaminaRegenSpeed(float InValue);
		
	UFUNCTION(BlueprintPure)
	float GetStaminaExpendSpeed() const;
	
	UFUNCTION(BlueprintCallable)
	void SetStaminaExpendSpeed(float InValue);

	UFUNCTION(BlueprintPure)
	virtual AVoxelChunk* GetOwnerChunk() const override { return OwnerChunk; }

	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerChunk(AVoxelChunk* InOwnerChunk) override { OwnerChunk = InOwnerChunk; }

	UFUNCTION(BlueprintPure)
	virtual FAbilityItem& GetGeneratingVoxelItem() override;

	UFUNCTION(BlueprintPure)
	virtual FVoxelItem& GetSelectedVoxelItem() override;

	UFUNCTION(BlueprintPure)
	ADWCharacter* GetOwnerRider() const { return OwnerRider; }

	UFUNCTION(BlueprintCallable)
	void SetOwnerRider(ADWCharacter* InOwnerRider) { this->OwnerRider = InOwnerRider; }

	UFUNCTION(BlueprintPure)
	ADWCharacter* GetRidingTarget() const { return RidingTarget; }

	UFUNCTION(BlueprintPure)
	ADWCharacter* GetLockedTarget() const { return LockedTarget; }

	UFUNCTION(BlueprintPure)
	FVector GetBirthLocation() const { return BirthLocation; }

	UFUNCTION(BlueprintPure)
	float GetAttackDistance() const { return AttackDistance; }

	UFUNCTION(BlueprintPure)
	float GetFollowDistance() const { return FollowDistance; }

	UFUNCTION(BlueprintPure)
	float GetPatrolDistance() const { return PatrolDistance; }

	UFUNCTION(BlueprintPure)
	float GetPatrolDuration() const { return PatrolDuration; }
	
	UFUNCTION(BlueprintPure)
	bool HasWeapon(EDWWeaponType InWeaponType);
		
	UFUNCTION(BlueprintPure)
	bool HasShield(EDWShieldType InShieldType);
		
	UFUNCTION(BlueprintPure)
	ADWEquipWeapon* GetWeapon();
	
	UFUNCTION(BlueprintPure)
	ADWEquipShield* GetShield();
		
	UFUNCTION(BlueprintPure)
	bool HasArmor(EDWEquipPartType InPartType);
	
	UFUNCTION(BlueprintPure)
	ADWEquipArmor* GetArmor(EDWEquipPartType InPartType);

	UFUNCTION(BlueprintPure)
	bool HasEquip(EDWEquipPartType InPartType);
	
	UFUNCTION(BlueprintPure)
	AAbilityEquipBase* GetEquip(EDWEquipPartType InPartType);
	
	UFUNCTION(BlueprintPure)
	TMap<EDWEquipPartType, AAbilityEquipBase*> GetEquips() const { return Equips; }

	UFUNCTION(BlueprintPure)
	FDWCharacterAttackAbilityData GetAttackAbility(int32 InAbilityIndex = -1);
		
	UFUNCTION(BlueprintPure)
	FDWCharacterSkillAbilityData GetSkillAbility(const FPrimaryAssetId& InSkillID);
		
	FDWCharacterSkillAbilityData GetSkillAbility(ESkillType InSkillType, int32 InAbilityIndex = -1);
			
	UFUNCTION(BlueprintPure)
	FDWCharacterActionAbilityData GetActionAbility(EDWCharacterActionType InActionType = EDWCharacterActionType::None);

	UFUNCTION(BlueprintPure)
	TArray<FDWCharacterAttackAbilityData> GetAttackAbilities() const { return AttackAbilities; }

	UFUNCTION(BlueprintPure)
	TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData> GetSkillAbilities() const { return SkillAbilities; }

	UFUNCTION(BlueprintPure)
	TMap<EDWCharacterActionType, FDWCharacterActionAbilityData> GetActionAbilities() const { return ActionAbilities; }

	UFUNCTION(BlueprintPure)
	UDWCharacterPart* GetCharacterPart(EDWCharacterPartType InCharacterPartType) const;

public:
	virtual void HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;
	
	virtual void HandleInterrupt(float InterruptDuration);
	
	virtual void HandleNameChanged(FName NewValue) override;

	virtual void HandleTeamIDChanged(FName NewValue);

	virtual void HandleRaceIDChanged(FName NewValue) override;

	virtual void HandleLevelChanged(int32 NewValue, int32 DeltaValue = 0) override;

	virtual void HandleEXPChanged(int32 NewValue, int32 DeltaValue = 0) override;

	virtual void HandleHealthChanged(float NewValue, float DeltaValue = 0.f) override;
	
	virtual void HandleMaxHealthChanged(float NewValue, float DeltaValue = 0.f) override;

	virtual void HandleManaChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleMaxManaChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleStaminaChanged(float NewValue, float DeltaValue = 0.f);
	
	virtual void HandleMaxStaminaChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleMoveSpeedChanged(float NewValue, float DeltaValue = 0.f) override;

	virtual void HandleSwimSpeedChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleRideSpeedChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleFlySpeedChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleRotationSpeedChanged(float NewValue, float DeltaValue = 0.f) override;

	virtual void HandleJumpForceChanged(float NewValue, float DeltaValue = 0.f) override;

	virtual void HandleDodgeForceChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleAttackForceChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleRepulseForceChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleAttackSpeedChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleAttackCritRateChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleAttackStealRateChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleDefendRateChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleDefendScopeChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandlePhysicsDefRateChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleMagicDefRateChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleToughnessRateChanged(float NewValue, float DeltaValue = 0.f);

	virtual void HandleRegenSpeedAttribute(float NewValue, float DeltaValue = 0.f);

	virtual void HandleExpendSpeedAttribute(float NewValue, float DeltaValue = 0.f);
};
