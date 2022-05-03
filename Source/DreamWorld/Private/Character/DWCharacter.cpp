// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Character/DWCharacter.h"

#include "TimerManager.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Components/DWAbilitySystemComponent.h"
#include "Character/DWCharacterAnim.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "AI/DWAIController.h"
#include "Character/DWCharacterPart.h"
#include "Global/GlobalBPLibrary.h"
#include "Inventory/CharacterInventory.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Scene/SceneModuleBPLibrary.h"
#include "Scene/Actor/PhysicsVolume/PhysicsVolumeBase.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/DWVoxelModule.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Widget/Components/WidgetCharacterHPComponent.h"
#include "Widget/World/WidgetCharacterHP.h"
#include "Ability/Character/DWCharacterAbility.h"
#include "Ability/Character/DWCharacterActionAbility.h"
#include "Ability/Character/DWCharacterAttackAbility.h"
#include "Ability/Character/DWCharacterAttributeSet.h"
#include "Ability/Character/DWCharacterSkillAbility.h"
#include "Ability/Item/ItemAbilityBase.h"
#include "Ability/Item/Equip/Armor/DWEquipArmor.h"
#include "Ability/Item/Equip/Shield/DWEquipShield.h"
#include "Ability/Item/Equip/Shield/DWEquipShieldData.h"
#include "Ability/Item/Equip/Weapon/DWEquipWeapon.h"
#include "Ability/Item/Equip/Weapon/DWEquipWeaponData.h"
#include "Ability/Item/Prop/DWPropData.h"
#include "Ability/Item/Prop/PropDataBase.h"
#include "Ability/Item/Skill/AbilitySkillBase.h"
#include "Ability/Item/Skill/DWSkillData.h"
#include "Ability/Item/Skill/SkillDataBase.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Character/DWCharacterData.h"
#include "Inventory/Slot/InventoryEquipSlot.h"
#include "Inventory/Slot/InventorySkillSlot.h"
#include "Main/MainModuleBPLibrary.h"
#include "Scene/Actor/PickUp/PickUp.h"
#include "Team/DWTeamModule.h"
#include "Voxel/Datas/VoxelData.h"
#include "Voxel/DWVoxelChunk.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"

//////////////////////////////////////////////////////////////////////////
// ADWCharacter

ADWCharacter::ADWCharacter()
{
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("StimuliSource"));
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSource->RegisterForSense(UAISense_Damage::StaticClass());

	WidgetCharacterHP = CreateDefaultSubobject<UWidgetCharacterHPComponent>(FName("WidgetCharacterHP"));
	WidgetCharacterHP->SetupAttachment(RootComponent);
	WidgetCharacterHP->SetRelativeLocation(FVector(0, 0, 70));

	AbilitySystem = CreateDefaultSubobject<UDWAbilitySystemComponent>(FName("AbilitySystem"));

	AttributeSet = CreateDefaultSubobject<UDWCharacterAttributeSet>(FName("AttributeSet"));

	Inventory = CreateDefaultSubobject<UCharacterInventory>(FName("Inventory"));

	BehaviorTree = nullptr;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DW_Character"));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCastShadow(false);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0);
	GetCharacterMovement()->JumpZVelocity = 420;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bComponentShouldUpdatePhysicsVolume = false;

	// states
	bDying = false;
	bActive = false;

	bFalling = false;
	bDodging = false;
	bSprinting = false;
	bCrouching = false;
	bSwimming = false;
	bFloating = false;
	bClimbing = false;
	bRiding = false;
	bFlying = false;
	bAttacking = false;
	bDefending = false;
	bDamaging = false;
	bInterrupting = false;
	bLockRotation = false;
	bFreeToAnimate = true;
	bBreakAllInput = false;

	// stats
	Nature = EDWCharacterNature::AIHostile;
	TeamID = NAME_None;
	AttackDistance = 100.f;
	InteractDistance = 500.f;
	FollowDistance = 500.f;
	PatrolDistance = 1000.f;
	PatrolDuration = 10.f;
	
	Equips = TMap<EDWEquipPartType, AAbilityEquipBase*>();
	for (int32 i = 0; i < 6; i++)
	{
		Equips.Add((EDWEquipPartType)i, nullptr);
	}

	OwnerChunk = nullptr;
	RidingTarget = nullptr;
	LockedTarget = nullptr;

	// local
	AttackAbilityIndex = 0;
	SkillAbilityID = FPrimaryAssetId();
	AttackType = EDWAttackType::None;
	ActionType = EDWCharacterActionType::None;
	BirthLocation = FVector(0, 0, 0);
	AIMoveLocation = Vector_Empty;
	AIMoveStopDistance = 0;
	InterruptRemainTime = 0;
	NormalAttackRemainTime = 0;

	DefaultAbility = FAbilityData();
	AttackAbilities = TArray<FDWCharacterAttackAbilityData>();
	SkillAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData>();
	ActionAbilities = TMap<EDWCharacterActionType, FDWCharacterActionAbilityData>();

	AIControllerClass = ADWAIController::StaticClass();
}

// Called when the game starts or when spawned
void ADWCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultGravityScale = GetCharacterMovement()->GravityScale;
	DefaultAirControl = GetCharacterMovement()->AirControl;

	AbilitySystem->InitAbilityActorInfo(this, this);

	BirthLocation = GetActorLocation();

	if (GetWidgetCharacterHPWidget() && !GetWidgetCharacterHPWidget()->GetOwnerCharacter())
	{
		GetWidgetCharacterHPWidget()->SetOwnerCharacter(this);
	}

	Spawn();
}

// Called every frame
void ADWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDead) return;

	if (bActive)
	{
		Inventory->Refresh(DeltaTime);
		
		if (InterruptRemainTime != -1)
		{
			InterruptRemainTime -= DeltaTime;
			if (InterruptRemainTime <= 0)
			{
				UnInterrupt();
			}
		}

		if(NormalAttackRemainTime > 0)
		{
			NormalAttackRemainTime -= DeltaTime;
		}

		if (LockedTarget)
		{
			if (!LockedTarget->IsDead() && FVector::Distance(GetActorLocation(), LockedTarget->GetActorLocation()) <= 1000)
			{
				LookAtTarget(LockedTarget);
			}
			else
			{
				SetLockedTarget(nullptr);
			}
		}

		if (AIMoveLocation != Vector_Empty)
		{
			if (DoAIMove(AIMoveLocation, AIMoveStopDistance))
			{
				StopAIMove();
			}
		}

		const FVector Location = GetMesh()->GetSocketLocation(FName("Foot"));

		if(AVoxelModule* VoxelModule = AVoxelModule::Get())
		{
			if(AVoxelChunk* Chunk = VoxelModule->FindChunk(Location))
			{
				if(Chunk != OwnerChunk)
				{
					if(OwnerChunk)
					{
						OwnerChunk->RemoveSceneActor(this);
					}
					Chunk->RemoveSceneActor(this);
				}
			}
			else if(OwnerChunk)
			{
				OwnerChunk->RemoveSceneActor(this);
			}
		}

		if (bSprinting && GetMoveDirection().Size() > 0.2f)
		{
			ModifyStamina(-GetStaminaExpendSpeed() * DeltaTime);
		}
		else if(IsFreeToAnim())
		{
			ModifyStamina(GetStaminaRegenSpeed() * DeltaTime);
		}
	}

	switch (GetCharacterMovement()->MovementMode)
	{
		case EMovementMode::MOVE_Walking:
		case EMovementMode::MOVE_Swimming:
		{
			if (bFalling)
			{
				FallEnd();
			}
			break;
		}
		case EMovementMode::MOVE_Falling:
		{
			if (!bFalling)
			{
				FallStart();
			}
			break;
		}
		case EMovementMode::MOVE_Flying:
		{
			bFalling = false;
			if(GetVelocity().Z < 0.f)
			{
				FFindFloorResult FindFloorResult;
				GetCharacterMovement()->FindFloor(GetCharacterMovement()->UpdatedComponent->GetComponentLocation(), FindFloorResult, GetVelocity().IsZero(), nullptr);
				if(FindFloorResult.IsWalkableFloor())
				{
					UnFly();
				}
			}
			break;
		}
		default: break;
	}
	if (GetActorLocation().Z < 0)
	{
		Death();
	}
	if (bDying && !bFalling && ActionType != EDWCharacterActionType::Death)
	{
		DeathStart();
	}
}

void ADWCharacter::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if(Ar.IsSaveGame())
	{
		float CurrentValue;
		if(Ar.IsLoading())
		{
			Ar << CurrentValue;
			if(CurrentValue > 0)
			{
				SetHealth(CurrentValue);
				
				Ar << CurrentValue;
				SetMana(CurrentValue);

				Ar << CurrentValue;
				SetStamina(CurrentValue);
			}
			else
			{
				Revive();
				Ar << CurrentValue;
				Ar << CurrentValue;
			}
		}
		else if(Ar.IsSaveGame())
		{
			CurrentValue = GetHealth();
			Ar << CurrentValue;

			CurrentValue = GetMana();
			Ar << CurrentValue;

			CurrentValue = GetStamina();
			Ar << CurrentValue;
		}
	}
}

void ADWCharacter::LoadData(FSaveData* InSaveData)
{
	auto SaveData = *static_cast<FDWCharacterSaveData*>(InSaveData);
	
	if (SaveData.bSaved)
	{
		AssetID = SaveData.ID;
		Nature = SaveData.Nature;
		SetNameV(SaveData.Name);
		SetRaceID(SaveData.RaceID);
		SetTeamID(SaveData.TeamID);
		SetLevelV(SaveData.Level);
		SetEXP(SaveData.EXP);
		AttackDistance = SaveData.AttackDistance;
		InteractDistance = SaveData.InteractDistance;
		FollowDistance = SaveData.FollowDistance;
		PatrolDistance = SaveData.PatrolDistance;
		PatrolDuration = SaveData.PatrolDuration;

		DefaultAbility = SaveData.DefaultAbility;
		FallingAttackAbility = SaveData.FallingAttackAbility;
		AttackAbilities = SaveData.AttackAbilities;
		SkillAbilities = SaveData.SkillAbilities;
		ActionAbilities = SaveData.ActionAbilities;

		Inventory->LoadData(SaveData.InventoryData, this);

		SetActorLocation(SaveData.SpawnLocation);
		SetActorRotation(SaveData.SpawnRotation);

		for(auto& iter : AttackAbilities)
		{
			iter.AbilityHandle = AcquireAbility(iter.AbilityClass, iter.AbilityLevel);
		}
		
		for(auto& iter : SkillAbilities)
		{
			iter.Value.AbilityHandle = AcquireAbility(iter.Value.AbilityClass, iter.Value.AbilityLevel);
		}

		for(auto& iter : ActionAbilities)
		{
			iter.Value.AbilityHandle = AcquireAbility(iter.Value.AbilityClass, iter.Value.AbilityLevel);
		}

		FallingAttackAbility.AbilityHandle = AcquireAbility(FallingAttackAbility.AbilityClass, FallingAttackAbility.AbilityLevel);

		DefaultAbility.AbilityHandle = AcquireAbility(GetCharacterData()->AbilityClass, DefaultAbility.AbilityLevel);
		ActiveAbility(DefaultAbility.AbilityHandle);

		UGlobalBPLibrary::LoadObjectFromMemory(this, SaveData.Datas);
	}
	else
	{
		AssetID = SaveData.ID;
		SetNameV(SaveData.Name);
		SetRaceID(SaveData.RaceID);
		SetLevelV(SaveData.Level);

		SetActorLocation(SaveData.SpawnLocation);
		SetActorRotation(SaveData.SpawnRotation);

		const UDWCharacterData* CharacterData = GetCharacterData<UDWCharacterData>();
		if(CharacterData->IsValid())
		{
			FString contextString;
			
			if (CharacterData->AttackAbilityTable != nullptr)
			{
				TArray<FDWCharacterAttackAbilityData*> attackAbilities;
				CharacterData->AttackAbilityTable->GetAllRows(contextString, attackAbilities);
				for (int i = 0; i < attackAbilities.Num(); i++)
				{
					auto abilityData = *attackAbilities[i];
					abilityData.AbilityHandle = AcquireAbility(abilityData.AbilityClass, abilityData.AbilityLevel);
					AttackAbilities.Add(abilityData);
				}
			}

			if (CharacterData->SkillAbilityTable != nullptr)
			{
				TArray<FDWCharacterSkillAbilityData*> skillAbilities;
				CharacterData->SkillAbilityTable->GetAllRows(contextString, skillAbilities);
				for (int i = 0; i < skillAbilities.Num(); i++)
				{
					auto abilityData = *skillAbilities[i];
					abilityData.AbilityHandle = AcquireAbility(abilityData.AbilityClass, abilityData.AbilityLevel);
					SkillAbilities.Add(abilityData.AbilityID, abilityData);
				}
			}

			if (CharacterData->ActionAbilityTable != nullptr)
			{
				TArray<FDWCharacterActionAbilityData*> actionAbilities;
				CharacterData->ActionAbilityTable->GetAllRows(contextString, actionAbilities);
				for (int i = 0; i < actionAbilities.Num(); i++)
				{
					auto abilityData = *actionAbilities[i];
					abilityData.AbilityHandle = AcquireAbility(abilityData.AbilityClass, abilityData.AbilityLevel);
					ActionAbilities.Add(abilityData.ActionType, abilityData);
				}
			}

			if (CharacterData->FallingAttackAbility.AbilityClass != nullptr)
			{
				FallingAttackAbility = CharacterData->FallingAttackAbility;
				FallingAttackAbility.AbilityHandle = AcquireAbility(FallingAttackAbility.AbilityClass, FallingAttackAbility.AbilityLevel);
			}

			if(CharacterData->AbilityClass != nullptr)
			{
				DefaultAbility = FAbilityData();
				DefaultAbility.AbilityLevel = SaveData.Level;
				DefaultAbility.AbilityHandle = AcquireAbility(CharacterData->AbilityClass, DefaultAbility.AbilityLevel);
				ActiveAbility(DefaultAbility.AbilityHandle);
			}

			Nature = CharacterData->Nature;
			AttackDistance = CharacterData->AttackDistance;
			InteractDistance = CharacterData->InteractDistance;
			FollowDistance = CharacterData->FollowDistance;
			PatrolDistance = CharacterData->PatrolDistance;
			PatrolDuration = CharacterData->PatrolDuration;
			
			SaveData.InventoryData = CharacterData->InventoryData;
		}

		// if(!IsPlayer())
		// {
		// 	const auto ItemDatas = UDWHelper::LoadItemDatas();
		// 	if(ItemDatas.Num() > 0 && FMath::FRand() < 0.5f)
		// 	{
		// 		SaveData.InventoryData.Items.Add(FAbilityItem(ItemDatas[FMath::RandRange(0, ItemDatas.Num() - 1)].ID, 1));
		// 	}
		// }

		Inventory->LoadData(SaveData.InventoryData, this);
	}
}

FSaveData* ADWCharacter::ToData(bool bSaved)
{
	static auto SaveData = FDWCharacterSaveData();

	SaveData.bSaved = bSaved;

	SaveData.ID = AssetID;
	SaveData.Nature = Nature;
	SaveData.Name = Name;
	SaveData.TeamID = TeamID;
	SaveData.RaceID = RaceID;
	SaveData.Level = Level;
	SaveData.EXP = EXP;
	SaveData.AttackDistance = AttackDistance;
	SaveData.InteractDistance = InteractDistance;
	SaveData.FollowDistance = FollowDistance;
	SaveData.PatrolDistance = PatrolDistance;
	SaveData.PatrolDuration = PatrolDuration;

	SaveData.InventoryData = Inventory->ToData();

	SaveData.DefaultAbility = DefaultAbility;
	SaveData.FallingAttackAbility = FallingAttackAbility;
	SaveData.AttackAbilities = AttackAbilities;
	SaveData.SkillAbilities = SkillAbilities;
	SaveData.ActionAbilities = ActionAbilities;

	SaveData.SpawnLocation = GetActorLocation();
	SaveData.SpawnRotation = GetActorRotation();

	UGlobalBPLibrary::SaveObjectToMemory(this, SaveData.Datas);

	return &SaveData;
}

void ADWCharacter::ResetData(bool bRefresh)
{
	// states
	bDead = false;
	bDying = false;
	bDodging = false;
	bSprinting = false;
	bCrouching = false;
	bSwimming = false;
	bFloating = false;
	bAttacking = false;
	bDefending = false;
	bDamaging = false;

	// stats
	SetMotionRate(1, 1);
	SetLockedTarget(nullptr);
	OwnerRider = nullptr;
	RidingTarget = nullptr;
			
	// local
	AttackAbilityIndex = 0;
	AIMoveLocation = Vector_Empty;
	AIMoveStopDistance = 0;
	InterruptRemainTime = 0;
	NormalAttackRemainTime = 0;
	ActionType = EDWCharacterActionType::None;

	if(bRefresh) RefreshData();
}

void ADWCharacter::RefreshData()
{
	HandleEXPChanged(GetEXP());
	HandleLevelChanged(GetLevelV());
	HandleHealthChanged(GetHealth());
	HandleManaChanged(GetMana());
	HandleStaminaChanged(GetStamina());
	HandleMoveSpeedChanged(GetMoveSpeed());
	HandleSwimSpeedChanged(GetSwimSpeed());
	HandleRideSpeedChanged(GetRideSpeed());
	HandleFlySpeedChanged(GetFlySpeed());
	HandleJumpForceChanged(GetJumpForce());
	HandleDodgeForceChanged(GetDodgeForce());
	HandleAttackForceChanged(GetAttackForce());
	HandleRepulseForceChanged(GetRepulseForce());
	HandleAttackSpeedChanged(GetAttackSpeed());
	HandleAttackCritRateChanged(GetAttackCritRate());
	HandleAttackStealRateChanged(GetAttackStealRate());
	HandleDefendRateChanged(GetDefendRate());
	HandlePhysicsDefRateChanged(GetPhysicsDefRate());
	HandleMagicDefRateChanged(GetMagicDefRate());
	HandleToughnessRateChanged(GetToughnessRate());
}

void ADWCharacter::Active(bool bResetData /*= false*/)
{
	if (!bActive)
	{
		bActive = true;
		UnInterrupt();
		GetCharacterMovement()->SetActive(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	if (bResetData) ResetData();
}

void ADWCharacter::Disable(bool bDisableMovement, bool bDisableCollision)
{
	if (bActive)
	{
		bActive = false;
		Interrupt();
		if (bDisableMovement)
		{
			GetCharacterMovement()->SetActive(false);
		}
		if (bDisableCollision)
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ADWCharacter::Spawn()
{
	Active(true);
	SetVisible(true);
	SetHealth(GetMaxHealth());
	SetMana(GetMaxMana());
	SetStamina(GetMaxStamina());
	DoAction(EDWCharacterActionType::Revive);
}

void ADWCharacter::Revive()
{
	if (bDead && !bDying)
	{
		Active(true);
		SetVisible(true);
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());
		DoAction(EDWCharacterActionType::Revive);
	}
}

void ADWCharacter::Death(AActor* InKiller /*= nullptr*/)
{
	if (!IsDead())
	{
		bDying = true;
		Disable(true);
		if(IAbilityVitalityInterface* InVitality = Cast<IAbilityVitalityInterface>(InKiller))
		{
			InVitality->ModifyEXP(GetTotalEXP());
		}
		SetEXP(0);
		SetHealth(0.f);
		SetMana(0.f);
		SetStamina(0.f);
		SetLockedTarget(nullptr);
		OnCharacterDead.Broadcast();
		if (!IsPlayer())
		{
			GetTeamData()->RemoveMember(this);
			GetController<ADWAIController>()->UnPossess();
		}
	}
}

void ADWCharacter::DeathStart()
{
	DoAction(EDWCharacterActionType::Death);
}

void ADWCharacter::DeathEnd()
{
	bDead = true;
	bDying = false;
	SetVisible(false);
	GetCharacterMovement()->SetActive(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(Inventory) Inventory->DiscardAllItem();
	if(!IsPlayer())
	{
		if(GetController())
		{
			GetController()->UnPossess();
		}
		if (OwnerChunk && OwnerChunk->IsValidLowLevel())
		{
			OwnerChunk->DestroySceneActor(this);
		}
		else
		{
			Destroy();
		}
		if (HasTeam())
		{
			GetTeamData()->RemoveMember(this);
		}
		for(auto Iter : Equips)
		{
			if(Iter.Value)
			{
				Iter.Value->Destroy();
			}
		}
	}
}

bool ADWCharacter::CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction)
{
	switch (InInteractAction)
	{
		case EInteractAction::Revive:
		{
			if(bDead)
			{
				return true;
			}
			break;
		}
		default: return true;
	}
	return false;
}

void ADWCharacter::OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction)
{
	switch (InInteractAction)
	{
		case EInteractAction::Revive:
		{
			Revive();
		}
		default: break;
	}
}

void ADWCharacter::FreeToAnim(bool bUnLockRotation /*= true*/)
{
	if (!bInterrupting)
	{
		bFreeToAnimate = true;
	}
	if (bUnLockRotation)
	{
		bLockRotation = false;
	}
}

void ADWCharacter::LimitToAnim(bool bInLockRotation /*= false*/, bool bUnSprint /*= false*/)
{
	if (!bFlying && !bFalling && !bRiding && !bClimbing && !bDefending)
	{
		bFreeToAnimate = false;
	}
	if (bInLockRotation)
	{
		bLockRotation = true;
	}
	if (bUnSprint)
	{
		UnSprint();
	}
}

void ADWCharacter::Interrupt(float InDuration /*= -1*/, bool bDoAction /*= false*/)
{
	if (!bInterrupting)
	{
		UnFly();
		UnRide();
		UnClimb();
		UnJump();
		UnDodge();
		UnAttack();
		UnDefend();
		UnSprint();
		LimitToAnim();
		bInterrupting = true;
		bBreakAllInput = true;
	}
	if (InDuration != -1) InDuration = InDuration * (1 - GetToughnessRate());
	InterruptRemainTime = InDuration;
	if (bDoAction) DoAction(EDWCharacterActionType::Interrupt);
}

void ADWCharacter::UnInterrupt()
{
	if (bInterrupting)
	{
		bInterrupting = false;
		bBreakAllInput = false;
		FreeToAnim();
		StopAction(EDWCharacterActionType::Interrupt);
	}
}

void ADWCharacter::Jump()
{
	if (IsFreeToAnim() && !bSwimming)
	{
		if(DoAction(EDWCharacterActionType::Jump))
		{
			Super::Jump();
		}
	}
}

void ADWCharacter::UnJump()
{
	if(bFalling)
	{
		StopAction(EDWCharacterActionType::Jump);
		Super::StopJumping();
	}
}

void ADWCharacter::Dodge()
{
	if (!bDodging && IsFreeToAnim() && GetMoveDirection().Size() > 0)
	{
		if (DoAction(EDWCharacterActionType::Dodge))
		{
			bDodging = true;
			LimitToAnim();
			GetCapsuleComponent()->SetGenerateOverlapEvents(false);
			SetActorRotation(FRotator(0.f, GetMoveDirection().ToOrientationRotator().Yaw, 0.f));
		}
	}
}

void ADWCharacter::UnDodge()
{
	if (bDodging)
	{
		bDodging = false;
		FreeToAnim();
		GetCapsuleComponent()->SetGenerateOverlapEvents(true);
		StopAction(EDWCharacterActionType::Dodge);
	}
}

void ADWCharacter::Sprint()
{
	if (!bSprinting && IsFreeToAnim())
	{
		bSprinting = true;
		HandleMoveSpeedChanged(GetMoveSpeed());
	}
}

void ADWCharacter::UnSprint()
{
	if (bSprinting)
	{
		bSprinting = false;
		HandleMoveSpeedChanged(GetMoveSpeed());
	}
}

void ADWCharacter::Crouch(bool bClientSimulation /*= false*/)
{
	if (!bCrouching && DoAction(EDWCharacterActionType::Crouch))
	{
		bCrouching = true;
		LimitToAnim();
		Super::Crouch(bClientSimulation);
	}
}

void ADWCharacter::UnCrouch(bool bClientSimulation /*= false*/)
{
	if (bCrouching)
	{
		bCrouching = false;
		FreeToAnim();
		StopAction(EDWCharacterActionType::Crouch);
		Super::UnCrouch(bClientSimulation);
	}
}

void ADWCharacter::Swim()
{
	if(!bSwimming && DoAction(EDWCharacterActionType::Swim))
	{
		bSwimming = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
 		if(USceneModuleBPLibrary::HasPhysicsVolumeByName(FName("Water")))
		{
			if(GetCharacterMovement()->UpdatedComponent)
			{
				GetCharacterMovement()->UpdatedComponent->SetPhysicsVolume(USceneModuleBPLibrary::GetPhysicsVolumeByName(FName("Water")), true);
			}
		}
		//FVector Velocity = GetMovementComponent()->Velocity;
		//GetMovementComponent()->Velocity = FVector(Velocity.X, Velocity.Y, 0);
	}
}

void ADWCharacter::UnSwim()
{
	if (bSwimming)
	{
		bSwimming = false;
		UnFloat();
		StopAction(EDWCharacterActionType::Swim);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		if(GetCharacterMovement()->UpdatedComponent)
		{
			GetCharacterMovement()->UpdatedComponent->SetPhysicsVolume(GetWorld()->GetDefaultPhysicsVolume(), true);
		}
		//FVector Velocity = GetMovementComponent()->Velocity;
		//GetMovementComponent()->Velocity = FVector(Velocity.X, Velocity.Y, 0);
	}
}

void ADWCharacter::Float(float InWaterPosZ)
{
	if(!bFloating && bSwimming)
	{
		bFloating = true;
		GetCharacterMovement()->Velocity = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, 0);
		const float NeckPosZ = GetCharacterPart(EDWCharacterPartType::Neck)->GetComponentLocation().Z;
		const float ChestPosZ = GetCharacterPart(EDWCharacterPartType::Chest)->GetComponentLocation().Z;
		const float OffsetZ = ChestPosZ + (NeckPosZ - ChestPosZ) * 0.35f - GetActorLocation().Z;
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, InWaterPosZ - OffsetZ));
	}
}

void ADWCharacter::UnFloat()
{
	if(bFloating)
	{
		bFloating = false;
		GetCharacterMovement()->Velocity = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, GetCharacterMovement()->Velocity.Z * 0.5f);
	}
}

void ADWCharacter::Climb()
{
	if (!bClimbing && DoAction(EDWCharacterActionType::Climb))
	{
		bClimbing = true;
		LimitToAnim();
	}
}

void ADWCharacter::UnClimb()
{
	if (bClimbing)
	{
		bClimbing = false;
		FreeToAnim();
		StopAction(EDWCharacterActionType::Climb);
	}
}

void ADWCharacter::Ride(ADWCharacter* InTarget)
{
	if (!bRiding && InTarget && DoAction(EDWCharacterActionType::Ride))
	{
		bRiding = true;
		RidingTarget = InTarget;
		if(GetOwnerController())
		{
			GetOwnerController()->Possess(RidingTarget);
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttachToComponent(RidingTarget->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("RiderPoint"));
		SetActorRelativeLocation(FVector::ZeroVector);
		SetActorRotation(RidingTarget->GetActorRotation());
		LimitToAnim();
	}
}

void ADWCharacter::UnRide()
{
	if (bRiding)
	{
		bRiding = false;
		FreeToAnim();
		if(IsActive()) GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		if(RidingTarget)
		{
			if(RidingTarget->GetOwnerController())
			{
				RidingTarget->GetOwnerController()->Possess(RidingTarget);
			}
			FHitResult hitResult;
			const FVector offset = GetActorRightVector() * (GetRadius() + RidingTarget->GetRadius());
			const FVector rayStart = FVector(GetActorLocation().X, GetActorLocation().Y, AVoxelModule::GetWorldData()->ChunkHeightRange * AVoxelModule::GetWorldData()->GetChunkLength() + 500) + offset;
			const FVector rayEnd = FVector(GetActorLocation().X, GetActorLocation().Y, 0) + offset;
			if (AVoxelModule::Get()->ChunkTraceSingle(rayStart, rayEnd, GetRadius(), GetHalfHeight(), hitResult))
			{
				SetActorLocation(hitResult.Location);
			}
		}
		RidingTarget = nullptr;
		GetOwnerController()->Possess(this);
		StopAction(EDWCharacterActionType::Ride);
	}
}

void ADWCharacter::Fly()
{
	if (!bFlying && DoAction(EDWCharacterActionType::Fly))
	{
		bFlying = true;
		LimitToAnim();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->Velocity = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, 100.f);
		// GetCharacterMovement()->GravityScale = 0.f;
		// GetCharacterMovement()->AirControl = 1.f;
	}
}

void ADWCharacter::UnFly()
{
	if (bFlying)
	{
		bFlying = false;
		FreeToAnim();
		StopAction(EDWCharacterActionType::Fly);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		// GetCharacterMovement()->GravityScale = DefaultGravityScale;
		// GetCharacterMovement()->AirControl = DefaultAirControl;
	}
}

bool ADWCharacter::Attack(int32 InAbilityIndex /*= -1*/)
{
	if (IsFreeToAnim() && NormalAttackRemainTime <= 0.f)
	{
		if(InAbilityIndex == -1) InAbilityIndex = AttackAbilityIndex;
		if (HasAttackAbility(InAbilityIndex) && HasWeapon(GetAttackAbility(InAbilityIndex).WeaponType))
		{
			if (ActiveAbility(GetAttackAbility(InAbilityIndex).AbilityHandle))
			{
				bAttacking = true;
				LimitToAnim(true, true);
				SetMotionRate(0, 0);
				AttackAbilityIndex = InAbilityIndex;
				AttackType = EDWAttackType::NormalAttack;
				NormalAttackRemainTime = 1.f / GetAttackSpeed();
				return true;
			}
			else
			{
				UnAttack();
			}
		}
	}
	else if(!bAttacking && bFalling)
	{
		return FallingAttack();
	}
	return false;
}

bool ADWCharacter::SkillAttack(const FPrimaryAssetId& InSkillID)
{
	if (IsFreeToAnim())
	{
		if (HasSkillAbility(InSkillID))
		{
			const auto AbilityData = GetSkillAbility(InSkillID);
			switch(AbilityData.GetItemData<USkillDataBase>()->SkillMode)
			{
				case ESkillMode::Initiative:
				{
					if(AbilityData.WeaponType == EDWWeaponType::None || AbilityData.WeaponType == GetWeapon()->GetItemData<UDWEquipWeaponData>()->WeaponType)
					{
						if(ActiveAbility(AbilityData.AbilityHandle))
						{
							bAttacking = true;
							LimitToAnim(true, true);
							SetMotionRate(0, 0);
							SkillAbilityID = InSkillID;
							AttackType = EDWAttackType::SkillAttack;
							return true;
						}
					}
					break;
				}
				case ESkillMode::Passive:
				{
					return true;
				}
				default: break;
			}
		}
	}
	return false;
}

bool ADWCharacter::SkillAttack(ESkillType InSkillType, int32 InAbilityIndex)
{
	if (IsFreeToAnim())
	{
		if (HasSkillAbility(InSkillType, InAbilityIndex))
		{
			const auto AbilityData = GetSkillAbility(InSkillType, InAbilityIndex);
			SkillAttack(AbilityData.AbilityID);
		}
	}
	return false;
}

bool ADWCharacter::FallingAttack()
{
	if (FallingAttackAbility.AbilityClass != nullptr && HasWeapon(FallingAttackAbility.WeaponType))
	{
		if (ActiveAbility(FallingAttackAbility.AbilityHandle))
		{
			bAttacking = true;
			LimitToAnim(true, true);
			SetMotionRate(0, 0);
			AttackType = EDWAttackType::FallingAttack;
			return true;
		}
	}
	return false;
}

void ADWCharacter::AttackStart()
{
	if (bAttacking)
	{
		switch (AttackType)
		{
			case EDWAttackType::NormalAttack:
			case EDWAttackType::FallingAttack:
			{
				SetDamaging(true);
				break;
			}
			case EDWAttackType::SkillAttack:
			{
				if (GetSkillAbility(SkillAbilityID).GetItemData<USkillDataBase>()->SkillClass != nullptr)
				{
					FActorSpawnParameters spawnParams = FActorSpawnParameters();
					spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					AAbilitySkillBase* tmpSkill = GetWorld()->SpawnActor<AAbilitySkillBase>(GetSkillAbility(SkillAbilityID).GetItemData<USkillDataBase>()->SkillClass, spawnParams);
					if(tmpSkill) tmpSkill->Initialize(this, SkillAbilityID);
				}
				break;
			}
			default: break;
		}
	}
}

void ADWCharacter::AttackHurt()
{
	if (bAttacking)
	{
		SetDamaging(false);
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this](){ SetDamaging(true); });
		GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
	}
}

void ADWCharacter::AttackEnd()
{
	if (bAttacking)
	{
		switch (AttackType)
		{
			case EDWAttackType::NormalAttack:
			{
				SetDamaging(false);
				if (++AttackAbilityIndex >= AttackAbilities.Num())
				{
					AttackAbilityIndex = 0;
				}
				break;
			}
			case EDWAttackType::SkillAttack:
			{
				UnAttack();
				break;
			}
			case EDWAttackType::FallingAttack:
			{
				UnAttack();
				StopAnimMontage();
				break;
			}
			default: break;
		}
	}
}

void ADWCharacter::UnAttack()
{
	if (bAttacking)
	{
		bAttacking = false;
		FreeToAnim();
		SetMotionRate(1, 1);
		SetDamaging(false);
		AttackAbilityIndex = 0;
		SkillAbilityID = FPrimaryAssetId();
		AttackType = EDWAttackType::None;
	}
}

void ADWCharacter::Defend()
{
	if (IsFreeToAnim() && DoAction(EDWCharacterActionType::Defend))
	{
		bDefending = true;
		SetMotionRate(0.5f, 0.1f);
		LimitToAnim(true, true);
	}
}

void ADWCharacter::UnDefend()
{
	if (bDefending)
	{
		bDefending = false;
		FreeToAnim();
		SetMotionRate(1, 1);
		StopAction(EDWCharacterActionType::Defend);
	}
}

void ADWCharacter::FallStart()
{
	bFalling = true;
	if(bDefending)
	{
		UnDefend();
	}
}

void ADWCharacter::FallEnd()
{
	bFalling = false;
	UnJump();
	if(bAttacking && AttackType == EDWAttackType::FallingAttack)
	{
		AttackEnd();
	}
}

bool ADWCharacter::UseItem(FAbilityItem& InItem)
{
	if(InItem.GetData()->EqualType(EAbilityItemType::Voxel))
	{
		FVoxelHitResult voxelHitResult;
		if (RaycastVoxel(voxelHitResult))
		{
			return GenerateVoxel(voxelHitResult, InItem);
		}
	}
	else if(InItem.GetData()->EqualType(EAbilityItemType::Prop))
	{
		const UDWPropData* PropData = InItem.GetData<UDWPropData>();
		switch (PropData->PropType)
		{
			case EDWPropType::Potion:
			{
				if(PropData->Name.ToString().StartsWith(TEXT("生命")))
				{
					return GetHealth() < GetMaxHealth();
				}
				else if(PropData->Name.ToString().StartsWith(TEXT("魔法")))
				{
					return GetMana() < GetMaxMana();
				}
				else if(PropData->Name.ToString().StartsWith(TEXT("体力")))
				{
					return GetStamina() < GetMaxStamina();
				}
				break;
			}
			case EDWPropType::Food:
			{
				return GetHealth() < GetMaxHealth() || GetStamina() < GetMaxStamina();
			}
			default: break;
		}
	}
	return false;
}

void ADWCharacter::PickUp(APickUp* InPickUp)
{
	if(InPickUp)
	{
		Inventory->AdditionItemByRange(InPickUp->GetItem(), -1);
	}
}

bool ADWCharacter::GenerateVoxel(const FVoxelHitResult& InVoxelHitResult, FAbilityItem& InItem)
{
	bool bSuccess = false;
	FQueryItemInfo QueryItemInfo;
	if(!IsPlayer())
	{
		QueryItemInfo = Inventory->GetItemInfoByRange(EQueryItemType::Remove, InItem);
	}
	else if(Inventory->GetSelectedItem().EqualType(InItem))
	{
		QueryItemInfo = FQueryItemInfo(Inventory->GetSelectedItem(), TArray<UInventorySlot*>{ Inventory->GetSelectedSlot() });
	}
	if(QueryItemInfo.Item.IsValid() && DoAction(EDWCharacterActionType::Generate))
	{
		AVoxelChunk* chunk = InVoxelHitResult.GetOwner();
		const FIndex index = chunk->LocationToIndex(InVoxelHitResult.Point - AVoxelModule::GetWorldData()->GetBlockSizedNormal(InVoxelHitResult.Normal)) + FIndex(InVoxelHitResult.Normal);
		const FVoxelItem& voxelItem = chunk->GetVoxelItem(index);

		if(!voxelItem.IsValid() || voxelItem.GetData<UVoxelData>()->Transparency == EVoxelTransparency::Transparent && voxelItem != InVoxelHitResult.VoxelItem)
		{
			const FVoxelItem _voxelItem = FVoxelItem(InItem.ID);

			//FRotator rotation = (Owner->VoxelIndexToLocation(index) + tmpVoxel->GetData<UVoxelData>()->GetCeilRange() * 0.5f * AVoxelModule::GetWorldInfo().BlockSize - UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>(this)->GetActorLocation()).ToOrientationRotator();
			//rotation = FRotator(FRotator::ClampAxis(FMath::RoundToInt(rotation.Pitch / 90) * 90.f), FRotator::ClampAxis(FMath::RoundToInt(rotation.Yaw / 90) * 90.f), FRotator::ClampAxis(FMath::RoundToInt(rotation.Roll / 90) * 90.f));
			//tmpVoxel->Rotation = rotation;

			FHitResult HitResult;
			if (!AVoxelModule::Get()->VoxelTraceSingle(_voxelItem, chunk->IndexToLocation(index), HitResult))
			{
				if(voxelItem.IsValid())
				{
					bSuccess = chunk->ReplaceVoxel(voxelItem, _voxelItem);
				}
				else
				{
					bSuccess = chunk->GenerateVoxel(index, _voxelItem);
				}
			}
		}
	}
	if(bSuccess) Inventory->RemoveItemBySlots(QueryItemInfo.Item, QueryItemInfo.Slots);
	return bSuccess;
}

bool ADWCharacter::DestroyVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(DoAction(EDWCharacterActionType::Destroy))
	{
		AVoxelChunk* chunk = InVoxelHitResult.GetOwner();
		const FVoxelItem& voxelItem = InVoxelHitResult.VoxelItem;

		if (voxelItem.GetData<UVoxelData>()->VoxelType != EVoxelType::Bedrock)
		{
			return chunk->DestroyVoxel(voxelItem);
		}
	}
	return false;
}

void ADWCharacter::RefreshEquip(EDWEquipPartType InPartType, UInventoryEquipSlot* EquipSlot)
{
	if (!EquipSlot->IsEmpty())
	{
		FActorSpawnParameters spawnParams = FActorSpawnParameters();
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Equips[InPartType] = GetWorld()->SpawnActor<AAbilityEquipBase>(EquipSlot->GetItem().GetData<UEquipDataBase>()->EquipClass, spawnParams);
		if (Equips[InPartType])
		{
			Equips[InPartType]->Initialize(this);
			Equips[InPartType]->OnAssemble();
		}
	}
	else if(Equips[InPartType])
	{
		Equips[InPartType]->OnDischarge();
		Equips[InPartType]->Destroy();
		Equips[InPartType] = nullptr;
	}
}

bool ADWCharacter::GetAbilityInfo(TSubclassOf<UAbilityBase> AbilityClass, FAbilityInfo& OutAbilityInfo)
{
	if (AbilitySystem && AbilityClass != nullptr)
	{
		float Cost = 0;
		float Cooldown = 0;
		EAbilityCostType CostType = EAbilityCostType::None;
		UAbilityBase* Ability = AbilityClass.GetDefaultObject();
		if (Ability->GetCostGameplayEffect()->Modifiers.Num() > 0)
		{
			const FGameplayModifierInfo ModifierInfo = Ability->GetCostGameplayEffect()->Modifiers[0];
			ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);
			if (ModifierInfo.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetHealthAttribute())
			{
				CostType = EAbilityCostType::Health;
			}
			else if (ModifierInfo.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetManaAttribute())
			{
				CostType = EAbilityCostType::Mana;
			}
			else if (ModifierInfo.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaAttribute())
			{
				CostType = EAbilityCostType::Stamina;
			}
		}
		Ability->GetCooldownGameplayEffect()->DurationMagnitude.GetStaticMagnitudeIfPossible(1, Cooldown);
		OutAbilityInfo = FAbilityInfo(CostType, Cost, Cooldown);
		return true;
	}
	return false;
}

bool ADWCharacter::DoAction(EDWCharacterActionType InActionType)
{
	if(!HasActionAbility(InActionType)) return false;

	const FDWCharacterActionAbilityData AbilityData = GetActionAbility(InActionType);
	if(!AbilityData.bNeedActive || IsActive(AbilityData.bNeedFreeToAnim))
	{
		if (StopAction(ActionType, true, true) && ActiveAbility(AbilityData.AbilityHandle))
		{
			ActionType = InActionType;
			return true;
		}
	}
	return false;
}

void ADWCharacter::EndAction(EDWCharacterActionType InActionType)
{
	switch(InActionType)
	{
		case EDWCharacterActionType::Death:
		{
			DeathEnd();
			break;
		}
		case EDWCharacterActionType::Jump:
		{
			UnJump();
			break;
		}
		case EDWCharacterActionType::Dodge:
		{
			UnDodge();
			break;
		}
		case EDWCharacterActionType::Crouch:
		{
			UnCrouch();
			break;
		}
		case EDWCharacterActionType::Defend:
		{
			UnDefend();
			break;
		}
		case EDWCharacterActionType::Fly:
		{
			UnFly();
			break;
		}
		case EDWCharacterActionType::Ride:
		{
			UnRide();
			break;
		}
		case EDWCharacterActionType::Climb:
		{
			UnClimb();
			break;
		}
		case EDWCharacterActionType::Swim:
		{
			UnSwim();
			break;
		}
		default: break;
	}
}

bool ADWCharacter::StopAction(EDWCharacterActionType InActionType, bool bCancelAbility, bool bEndAction)
{
	if (InActionType == EDWCharacterActionType::None) InActionType = ActionType;

	if (ActionType == EDWCharacterActionType::None || ActionType != InActionType) return true;

	if (bCancelAbility)
	{
		const FDWCharacterActionAbilityData AbilityData = GetActionAbility(InActionType);
		if (!AbilityData.bCancelable) return false;

		CancelAbilityByHandle(AbilityData.AbilityHandle);
	}

	ActionType = EDWCharacterActionType::None;

	if (bEndAction)
	{
		EndAction(InActionType);
	}
	return true;
}

void ADWCharacter::ModifyMana(float InDeltaValue)
{
	if(InDeltaValue < 0.f && GetMana() > 0.f || InDeltaValue > 0.f && GetMana() < GetMaxMana())
	{
		AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetManaAttribute(), EGameplayModOp::Additive, InDeltaValue);
	}
}

void ADWCharacter::ModifyStamina(float InDeltaValue)
{
	if(InDeltaValue < 0.f && GetStamina() > 0.f || InDeltaValue > 0.f && GetStamina() < GetMaxStamina())
	{
		AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaAttribute(), EGameplayModOp::Additive, InDeltaValue);
	}
}

void ADWCharacter::SetLockedTarget(ADWCharacter* InTargetCharacter)
{
	if(InTargetCharacter)
	{
		LockedTarget = InTargetCharacter;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		LockedTarget = nullptr;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void ADWCharacter::LookAtTarget(ADWCharacter* InTargetCharacter)
{
	if(InTargetCharacter && !bDodging)
	{
		const FVector tmpDirection = InTargetCharacter->GetActorLocation() - GetActorLocation();
		SetActorRotation(FRotator(0, tmpDirection.ToOrientationRotator().Yaw, 0));
	}
}

void ADWCharacter::AIMoveTo(FVector InTargetLocation, float InMoveStopDistance /*= 10*/, bool bMulticast /*= false*/)
{
	AIMoveLocation = InTargetLocation;
	AIMoveStopDistance = InMoveStopDistance;
}

bool ADWCharacter::DoAIMove(FVector InTargetLocation, float InMoveStopDistance /*= 10*/)
{
	if (FVector::Distance(FVector(GetActorLocation().X, GetActorLocation().Y, 0), FVector(InTargetLocation.X, InTargetLocation.Y, 0)) > InMoveStopDistance)
	{
		AddMovementInput(InTargetLocation - GetActorLocation());
		return false;
	}
	return true;
}

bool ADWCharacter::DoAIMove(ADWCharacter* InTargetCharacter, float InMoveStopDistance /*= 10*/, bool bLookAtTarget /*= false*/)
{
	if(!InTargetCharacter || !InTargetCharacter->IsValidLowLevel()) return false;

	if (Distance(InTargetCharacter, false, false) > InMoveStopDistance)
	{
		AddMovementInput(InTargetCharacter->GetActorLocation() - GetActorLocation());
		if (bLookAtTarget) SetLockedTarget(InTargetCharacter);
		return false;
	}
	if (bLookAtTarget) SetLockedTarget(nullptr);
	return true;
}

void ADWCharacter::StopAIMove(bool bMulticast /*= false*/)
{
	AIMoveLocation = Vector_Empty;
}

void ADWCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if (bBreakAllInput) return;

	switch(GetCharacterMovement()->MovementMode)
	{
		case EMovementMode::MOVE_Swimming:
		{
			if(bFloating && WorldDirection.Z > -0.5f)
			{
				WorldDirection.Z = 0;
			}
			break;
		}
		default: break;
	}

	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);

	if (!IsPlayerControlled())
	{
		FHitResult hitResult;
		if (RaycastStep(hitResult))
		{
			Jump();
		}
	}
}

void ADWCharacter::SetDamaging(bool bInDamaging)
{
	bDamaging = bInDamaging;
}

UInventory* ADWCharacter::GetInventory() const
{
	return Inventory;
}

bool ADWCharacter::HasBehaviorTree() const
{
	return GetCharacterData<UDWCharacterData>()->BehaviorTreeAsset != nullptr;
}

UBehaviorTree* ADWCharacter::GetBehaviorTree()
{
	const UDWCharacterData* CharacterData = GetCharacterData<UDWCharacterData>();
	if (!BehaviorTree)
	{
		BehaviorTree = DuplicateObject<UBehaviorTree>(CharacterData->BehaviorTreeAsset, this);
		if(BehaviorTree)
		{
			UBlackboardData* Blackboard = DuplicateObject<UBlackboardData>(CharacterData->BehaviorTreeAsset->BlackboardAsset, nullptr);
			BehaviorTree->BlackboardAsset = Blackboard;
		}
	}
	return BehaviorTree;
}

UWidgetCharacterHP* ADWCharacter::GetWidgetCharacterHPWidget() const
{
	if (WidgetCharacterHP->GetUserWidgetObject())
	{
		return Cast<UWidgetCharacterHP>(WidgetCharacterHP->GetUserWidgetObject());
	}
	return nullptr;
}

bool ADWCharacter::IsActive(bool bNeedFreeToAnim /*= false*/) const
{
	return bActive && !IsDead() && (!bNeedFreeToAnim || IsFreeToAnim(false));
}

bool ADWCharacter::IsFreeToAnim(bool bCheckStates) const
{
	return bFreeToAnimate && (!bCheckStates || !bFlying && !bFalling && !bRiding && !bClimbing && !bDefending);
}

bool ADWCharacter::IsDead() const
{
	return bDead || bDying;
}

FDWTeamData* ADWCharacter::GetTeamData() const
{
	if(ADWTeamModule* TeamModule = AMainModule::GetModuleByClass<ADWTeamModule>())
	{
		return TeamModule->GetTeamData(TeamID);
	}
	return nullptr;
}

bool ADWCharacter::IsTargetable_Implementation() const
{
	return !IsDead();
}

void ADWCharacter::SetTeamID(FName InTeamID)
{
	TeamID = InTeamID;
	HandleTeamIDChanged(InTeamID);
}

float ADWCharacter::GetMana() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetMana();
}

void ADWCharacter::SetMana(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetManaAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetMaxMana() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxMana();
}

void ADWCharacter::SetMaxMana(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxManaAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetStamina() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetStamina();
}

void ADWCharacter::SetStamina(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetMaxStamina() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxStamina();
}

void ADWCharacter::SetMaxStamina(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxStaminaAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetSwimSpeed() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetSwimSpeed();
}

void ADWCharacter::SetSwimSpeed(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetSwimSpeedAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetRideSpeed() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetRideSpeed();
}

void ADWCharacter::SetRideSpeed(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetRideSpeedAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetFlySpeed() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetFlySpeed();
}

void ADWCharacter::SetFlySpeed(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetFlySpeedAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetDodgeForce() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetDodgeForce();
}

void ADWCharacter::SetDodgeForce(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetDodgeForceAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetAttackForce() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackForce();
}

void ADWCharacter::SetAttackForce(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackForceAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetRepulseForce() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetRepulseForce();
}

void ADWCharacter::SetRepulseForce(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetRepulseForceAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetAttackSpeed() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackSpeed();
}

void ADWCharacter::SetAttackSpeed(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackSpeedAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetAttackCritRate() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackCritRate();
}

void ADWCharacter::SetAttackCritRate(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackCritRateAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetAttackStealRate() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackStealRate();
}

void ADWCharacter::SetAttackStealRate(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackStealRateAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetDefendRate() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetDefendRate();
}

void ADWCharacter::SetDefendRate(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetDefendRateAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetDefendScope() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetDefendScope();
}

void ADWCharacter::SetDefendScope(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetDefendScopeAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetPhysicsDefRate() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetPhysicsDefRate();
}

void ADWCharacter::SetPhysicsDefRate(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetPhysicsDefRateAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetMagicDefRate() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetMagicDefRate();
}

void ADWCharacter::SetMagicDefRate(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetMagicDefRateAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetToughnessRate() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetToughnessRate();
}

void ADWCharacter::SetToughnessRate(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetToughnessRateAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetStaminaRegenSpeed() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaRegenSpeed();
}

void ADWCharacter::SetStaminaRegenSpeed(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaRegenSpeedAttribute(), EGameplayModOp::Override, InValue);
}

float ADWCharacter::GetStaminaExpendSpeed() const
{
	return GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaExpendSpeed();
}

void ADWCharacter::SetStaminaExpendSpeed(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaExpendSpeedAttribute(), EGameplayModOp::Override, InValue);
}

FAbilityItem& ADWCharacter::GetGeneratingVoxelItem()
{
	FAbilityItem tmpItem = Inventory->GetSelectedItem();
	if(tmpItem.IsValid() && tmpItem.GetData()->EqualType(EAbilityItemType::Voxel))
	{
		return tmpItem;
	}
	return FAbilityItem::Empty;
}

FVoxelItem& ADWCharacter::GetSelectedVoxelItem()
{
	return SelectedVoxelItem;
}

bool ADWCharacter::HasWeapon(EDWWeaponType InWeaponType)
{
	if(InWeaponType == EDWWeaponType::None) return true;
	
	if(HasEquip(EDWEquipPartType::RightHand))
	{
		if(ADWEquipWeapon* Weapon = Cast<ADWEquipWeapon>(GetEquip(EDWEquipPartType::RightHand)))
		{
			return Weapon->GetItemData<UDWEquipWeaponData>()->WeaponType == InWeaponType;
		}
	}
	return false;
}

bool ADWCharacter::HasShield(EDWShieldType InShieldType)
{
	if(InShieldType == EDWShieldType::None) return true;
	
	if(HasEquip(EDWEquipPartType::LeftHand))
	{
		if(ADWEquipShield* Weapon = Cast<ADWEquipShield>(GetEquip(EDWEquipPartType::LeftHand)))
		{
			return Weapon->GetItemData<UDWEquipShieldData>()->ShieldType == InShieldType;
		}
	}
	return false;
}

ADWEquipWeapon* ADWCharacter::GetWeapon()
{
	return Cast<ADWEquipWeapon>(GetEquip(EDWEquipPartType::RightHand));
}

ADWEquipShield* ADWCharacter::GetShield()
{
	return Cast<ADWEquipShield>(GetEquip(EDWEquipPartType::LeftHand));
}

bool ADWCharacter::HasArmor(EDWEquipPartType InPartType)
{
	return HasEquip(InPartType) && GetEquip(InPartType)->IsA(ADWEquipArmor::StaticClass());
}

ADWEquipArmor* ADWCharacter::GetArmor(EDWEquipPartType InPartType)
{
	if (HasArmor(InPartType))
	{
		return Cast<ADWEquipArmor>(GetEquip(InPartType));
	}
	return nullptr;
}

bool ADWCharacter::HasEquip(EDWEquipPartType InPartType)
{
	return Equips.Contains(InPartType) && Equips[InPartType];
}

AAbilityEquipBase* ADWCharacter::GetEquip(EDWEquipPartType InPartType)
{
	if (HasEquip(InPartType))
	{
		return Equips[InPartType];
	}
	return nullptr;
}

FDWCharacterAttackAbilityData ADWCharacter::GetAttackAbility(int32 InAbilityIndex /*= -1*/)
{
	if (HasAttackAbility(InAbilityIndex == -1 ? AttackAbilityIndex : InAbilityIndex))
	{
		return AttackAbilities[InAbilityIndex == -1 ? AttackAbilityIndex : InAbilityIndex];
	}
	return FDWCharacterAttackAbilityData();
}

FDWCharacterSkillAbilityData ADWCharacter::GetSkillAbility(const FPrimaryAssetId& InSkillID)
{
	if(HasSkillAbility(InSkillID))
	{
		return SkillAbilities[InSkillID];
	}
	return FDWCharacterSkillAbilityData();
}

FDWCharacterSkillAbilityData ADWCharacter::GetSkillAbility(ESkillType InSkillType, int32 InAbilityIndex)
{
	if(HasSkillAbility(InSkillType, InAbilityIndex))
	{
		TArray<FDWCharacterSkillAbilityData> Abilities = TArray<FDWCharacterSkillAbilityData>();
		for (auto Iter : SkillAbilities)
		{
			if(Iter.Value.GetItemData<USkillDataBase>()->SkillType == InSkillType)
			{
				Abilities.Add(Iter.Value);
			}
		}
		return InAbilityIndex != -1 ? Abilities[InAbilityIndex] : Abilities[0];
	}
	return FDWCharacterSkillAbilityData();
}

FDWCharacterActionAbilityData ADWCharacter::GetActionAbility(EDWCharacterActionType InActionType /*= ECharacterActionType::None*/)
{
	if(InActionType == EDWCharacterActionType::None) InActionType = ActionType;
	if (HasActionAbility(InActionType))
	{
		return ActionAbilities[InActionType];
	}
	return FDWCharacterActionAbilityData();
}

bool ADWCharacter::RaycastStep(FHitResult& OutHitResult)
{
	const FVector rayStart = GetActorLocation() + FVector::DownVector * (GetHalfHeight() - GetCharacterMovement()->MaxStepHeight);
	const FVector rayEnd = rayStart + GetMoveDirection() * (GetRadius() + AVoxelModule::GetWorldData()->BlockSize * FMath::Clamp(GetMoveDirection().Size() * 0.005f, 0.5f, 1.3f));
	return UKismetSystemLibrary::LineTraceSingle(this, rayStart, rayEnd, UDWHelper::GetGameTrace(EDWGameTraceType::Step), false, TArray<AActor*>(), EDrawDebugTrace::None, OutHitResult, true);
}

bool ADWCharacter::RaycastVoxel(FVoxelHitResult& OutHitResult)
{
	FHitResult hitResult;
	const FVector rayStart = GetActorLocation();
	const FVector rayEnd = rayStart + GetActorForwardVector() * InteractDistance;
	if (UKismetSystemLibrary::LineTraceSingle(this, rayStart, rayEnd, UDWHelper::GetGameTrace(EDWGameTraceType::Voxel), false, TArray<AActor*>(), EDrawDebugTrace::None, hitResult, true))
	{
		if (hitResult.GetActor()->IsA(AVoxelChunk::StaticClass()))
		{
			AVoxelChunk* chunk = Cast<AVoxelChunk>(hitResult.GetActor());
			if (chunk != nullptr)
			{
				const FVoxelItem& voxelItem = chunk->GetVoxelItem(chunk->LocationToIndex(hitResult.ImpactPoint - AVoxelModule::GetWorldData()->GetBlockSizedNormal(hitResult.ImpactNormal, 0.01f)));
				if (voxelItem.IsValid())
				{
					OutHitResult = FVoxelHitResult(voxelItem, hitResult.ImpactPoint, hitResult.ImpactNormal);
					return true;
				}
			}
		}
	}
	return false;
}

bool ADWCharacter::HasTeam() const
{
	return GetTeamData()->IsValid();
}

bool ADWCharacter::IsTeamMate(ADWCharacter* InTargetCharacter) const
{
	return HasTeam() && InTargetCharacter->TeamID == TeamID;
}

bool ADWCharacter::HasAttackAbility(int32 InAbilityIndex /*= -1*/) const
{
	if (InAbilityIndex != -1)
	{
		return AttackAbilities.Num() > InAbilityIndex;
	}
	else
	{
		return AttackAbilities.Num() > 0;
	}
}

bool ADWCharacter::HasSkillAbility(const FPrimaryAssetId& InSkillID)
{
	if(SkillAbilities.Contains(InSkillID))
	{
		auto SkillSlots = Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill);
		for (int32 i = 0; i < SkillSlots.Num(); i++)
		{
			if(SkillSlots[i]->GetItem().ID == InSkillID)
			{
				return true;
			}
		}
	}
	return false;
}

bool ADWCharacter::HasSkillAbility(ESkillType InSkillType, int32 InAbilityIndex)
{
	TArray<FDWCharacterSkillAbilityData> Abilities = TArray<FDWCharacterSkillAbilityData>();
	for (auto Iter : SkillAbilities)
	{
		if(Iter.Value.GetItemData<UDWSkillData>()->SkillType == InSkillType)
		{
			Abilities.Add(Iter.Value);
		}
	}
	if(InAbilityIndex != -1 ? Abilities.IsValidIndex(InAbilityIndex) : Abilities.Num() > 0)
	{
		auto SkillSlots = Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill);
		for (int32 i = 0; i < SkillSlots.Num(); i++)
		{
			if(SkillSlots[i]->GetItem().ID == Abilities[InAbilityIndex != -1 ? InAbilityIndex : 0].AbilityID)
			{
				return true;
			}
		}
	}
	return false;
}

bool ADWCharacter::HasActionAbility(EDWCharacterActionType InActionType)
{
	if (ActionAbilities.Contains(InActionType))
	{
		return true;
	}
	return false;
}

bool ADWCharacter::CreateTeam(const FName& InTeamName /*= MANE_None*/, FString InTeamDetail /*= TEXT("")*/)
{
	if(ADWTeamModule* TeamModule = AMainModule::GetModuleByClass<ADWTeamModule>())
	{
		return TeamModule->CreateTeam(this, InTeamName, InTeamDetail);
	}
	return false;
}

bool ADWCharacter::DissolveTeam()
{
	if(ADWTeamModule* TeamModule = AMainModule::GetModuleByClass<ADWTeamModule>())
	{
		return TeamModule->DissolveTeam(TeamID, this);
	}
	return false;
}

bool ADWCharacter::JoinTeam(const FName& InTeamID)
{
	if(ADWTeamModule* TeamModule = AMainModule::GetModuleByClass<ADWTeamModule>())
	{
		if(TeamModule->IsExistTeam(InTeamID))
		{
			TeamModule->GetTeamData(InTeamID)->AddMember(this);
			return true;
		}
	}
	return false;
}

bool ADWCharacter::JoinTeam(ADWCharacter* InTargetCharacter)
{
	return JoinTeam(InTargetCharacter->GetTeamID());
}

bool ADWCharacter::LeaveTeam()
{
	if (HasTeam())
	{
		GetTeamData()->RemoveMember(this);
		return true;
	}
	return false;
}

bool ADWCharacter::AddTeamMate(ADWCharacter* InTargetCharacter)
{
	if (HasTeam() && GetTeamData()->IsCaptain(this))
	{
		GetTeamData()->AddMember(InTargetCharacter);
		return true;
	}
	return false;
}

bool ADWCharacter::RemoveTeamMate(ADWCharacter* InTargetCharacter)
{
	if (HasTeam() && GetTeamData()->IsCaptain(this))
	{
		GetTeamData()->RemoveMember(InTargetCharacter);
		return true;
	}
	return false;
}

TArray<ADWCharacter*> ADWCharacter::GetTeamMates()
{
	return GetTeamData()->GetMembers(this);
}

bool ADWCharacter::IsPlayer() const
{
	return Nature == EDWCharacterNature::Player;
}

bool ADWCharacter::IsEnemy(ADWCharacter* InTargetCharacter) const
{
	switch (Nature)
	{
		case EDWCharacterNature::NPC:
		case EDWCharacterNature::AIFriendly:
		{
			return false;
		}
		default:
		{
			switch (InTargetCharacter->Nature)
			{
				case EDWCharacterNature::NPC:
				case EDWCharacterNature::AIFriendly:
				{
					return false;
				}
				default: break;
			}
			break;
		}
	}
	return !IsTeamMate(InTargetCharacter) && !InTargetCharacter->GetRaceID().IsEqual(RaceID);
}

void ADWCharacter::SetVisible_Implementation(bool bVisible)
{
	Super::SetVisible_Implementation(bVisible);
}

void ADWCharacter::SetMotionRate_Implementation(float InMovementRate, float InRotationRate)
{
	MovementRate = InMovementRate;
	RotationRate = InRotationRate;
	HandleMoveSpeedChanged(GetMoveSpeed());
	HandleRotationSpeedChanged(GetRotationSpeed());
	HandleSwimSpeedChanged(GetSwimSpeed());
	HandleRideSpeedChanged(GetRideSpeed());
	HandleFlySpeedChanged(GetFlySpeed());
}

UDWCharacterPart* ADWCharacter::GetCharacterPart(EDWCharacterPartType InCharacterPartType) const
{
	TArray<UDWCharacterPart*> Parts;
	GetComponents(Parts);
	for(auto Iter : Parts)
	{
		if(Iter->GetCharacterPartType() == InCharacterPartType)
		{
			return Iter;
		}
	}
	return nullptr;
}

void ADWCharacter::HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleDamage(DamageType, LocalDamageDone, bHasCrited, HitResult, SourceTags, SourceActor);
	
	if(SourceActor && SourceActor != this)
	{
		if(ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor))
		{
			if(DamageType == EDamageType::Physics)
			{
				SourceCharacter->ModifyHealth(LocalDamageDone * SourceCharacter->GetAttackStealRate());
			}
			if(!IsDead() && !IsPlayer())
			{
				if(!GetController<ADWAIController>()->GetTargetCharacter())
				{
					GetController<ADWAIController>()->SetTargetCharacter(SourceCharacter);
				}
			}
		}
	}
}

void ADWCharacter::HandleInterrupt(float InterruptDuration)
{
	if (!FMath::IsNearlyZero(InterruptDuration))
	{
		Interrupt(InterruptDuration * (1 - GetToughnessRate()), true);
	}
}

void ADWCharacter::HandleNameChanged(FName NewValue)
{
	Super::HandleNameChanged(NewValue);
	
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::HandleTeamIDChanged(FName NewValue)
{
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::HandleRaceIDChanged(FName NewValue)
{
	Super::HandleRaceIDChanged(NewValue);
	
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::HandleLevelChanged(int32 NewValue, int32 DeltaValue /*= 0*/)
{
	Super::HandleLevelChanged(NewValue, DeltaValue);
	
	FallingAttackAbility.AbilityLevel = NewValue;
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::HandleEXPChanged(int32 NewValue, int32 DeltaValue /*= 0*/)
{
	Super::HandleEXPChanged(NewValue, DeltaValue);
	
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::HandleHealthChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	Super::HandleHealthChanged(NewValue, DeltaValue);
	
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHealthPercent(NewValue, GetMaxHealth());
	}
}

void ADWCharacter::HandleMaxHealthChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	Super::HandleMaxHealthChanged(NewValue, DeltaValue);
	
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHealthPercent(NewValue, GetMaxHealth());
	}
}

void ADWCharacter::HandleManaChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetManaPercent(NewValue, GetMaxMana());
	}
}

void ADWCharacter::HandleMaxManaChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetManaPercent(NewValue, GetMaxMana());
	}
}

void ADWCharacter::HandleStaminaChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetStaminaPercent(NewValue, GetMaxStamina());
	}
}

void ADWCharacter::HandleMaxStaminaChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetStaminaPercent(NewValue, GetMaxStamina());
	}
}

void ADWCharacter::HandleMoveSpeedChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	GetCharacterMovement()->MaxWalkSpeed = NewValue * (bSprinting ? 1.5f : 1) * MovementRate;
}

void ADWCharacter::HandleSwimSpeedChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	GetCharacterMovement()->MaxSwimSpeed = NewValue * (bSprinting ? 1.5f : 1) * MovementRate;
}

void ADWCharacter::HandleRideSpeedChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	GetCharacterMovement()->MaxWalkSpeed = NewValue * (bSprinting ? 1.5f : 1) * MovementRate;
}

void ADWCharacter::HandleFlySpeedChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	GetCharacterMovement()->MaxFlySpeed = NewValue * (bSprinting ? 1.5f : 1) * MovementRate;
}

void ADWCharacter::HandleRotationSpeedChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	GetCharacterMovement()->RotationRate = FRotator(0, NewValue * (bSprinting ? 1.5f : 1), 0) * RotationRate;
}

void ADWCharacter::HandleJumpForceChanged(float NewValue, float DeltaValue)
{
	Super::HandleJumpForceChanged(NewValue, DeltaValue);
}

void ADWCharacter::HandleDodgeForceChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleAttackForceChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleRepulseForceChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleAttackSpeedChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleAttackCritRateChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleAttackStealRateChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleDefendRateChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleDefendScopeChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandlePhysicsDefRateChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleMagicDefRateChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleToughnessRateChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	
}

void ADWCharacter::HandleRegenSpeedAttribute(float NewValue, float DeltaValue /*= 0.f*/)
{

}

void ADWCharacter::HandleExpendSpeedAttribute(float NewValue, float DeltaValue /*= 0.f*/)
{

}
