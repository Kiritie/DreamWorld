// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Character/DWCharacter.h"

#include "TimerManager.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Abilities/ItemAbilityBase.h"
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
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Item/Equip/Armor/DWEquipArmor.h"
#include "Item/Equip/Shield/DWEquipShield.h"
#include "Item/Equip/Shield/DWEquipShieldData.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "Item/Equip/Weapon/DWEquipWeaponData.h"
#include "Item/Prop/DWPropData.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Ability/Item/Skill/AbilitySkillBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/PickUp/AbilityPickUpBase.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Character/CharacterModuleBPLibrary.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Character/States/DWCharacterState_Climb.h"
#include "Character/States/DWCharacterState_Crouch.h"
#include "Character/States/DWCharacterState_Default.h"
#include "Character/States/DWCharacterState_Defend.h"
#include "Character/States/DWCharacterState_Dodge.h"
#include "Character/States/DWCharacterState_Float.h"
#include "Character/States/DWCharacterState_Fly.h"
#include "Character/States/DWCharacterState_Interrupt.h"
#include "Character/States/DWCharacterState_Ride.h"
#include "Character/States/DWCharacterState_Swim.h"
#include "FSM/Components/FSMComponent.h"
#include "Inventory/Slot/InventoryEquipSlot.h"
#include "Inventory/Slot/InventorySkillSlot.h"
#include "Item/Prop/DWPropData.h"
#include "Main/MainModuleBPLibrary.h"
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
	Inventory->GetOnSlotSelected().AddDynamic(this, &ADWCharacter::OnInventorySlotSelected);

	BehaviorTree = nullptr;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCastShadow(false);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0);
	GetCharacterMovement()->JumpZVelocity = 420;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bComponentShouldUpdatePhysicsVolume = false;

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

	RidingTarget = nullptr;
	LockedTarget = nullptr;

	// local
	AttackAbilityIndex = 0;
	SkillAbilityID = FPrimaryAssetId();
	AttackType = EDWCharacterAttackType::None;
	ActionType = EDWCharacterActionType::None;
	BirthLocation = FVector(0, 0, 0);
	AIMoveLocation = Vector_Empty;
	AIMoveStopDistance = 0;
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
}

void ADWCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if(!FSM->GetCurrentState() || FSM->IsCurrentStateClass<UDWCharacterState_Default>())
	{
		switch (GetCharacterMovement()->MovementMode)
		{
			case EMovementMode::MOVE_Flying:
			{
				FSM->SwitchStateByClass<UDWCharacterState_Fly>();
				break;
			}
			case EMovementMode::MOVE_Swimming:
			{
				FSM->SwitchStateByClass<UDWCharacterState_Swim>();
				break;
			}
			default: break;
		}
	}
}

void ADWCharacter::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void ADWCharacter::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();
}

// Called every frame
void ADWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;

	if (IsActive())
	{
		Inventory->Refresh(DeltaTime);
		
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

		if(AVoxelModule* VoxelModule = AMainModule::GetModuleByClass<AVoxelModule>())
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

		if (IsSprinting() && GetMoveDirection().Size() > 0.2f)
		{
			ModifyStamina(-GetStaminaExpendSpeed() * DeltaTime);
		}
		else if(IsFreeToAnim())
		{
			ModifyStamina(GetStaminaRegenSpeed() * DeltaTime);
		}

		if (GetActorLocation().Z < 0)
		{
			Death();
		}
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
	auto SaveData = InSaveData->ToRef<FDWCharacterSaveData>();
	
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

		DefaultAbility.AbilityHandle = AcquireAbility(GetCharacterData<UDWCharacterData>().AbilityClass, DefaultAbility.AbilityLevel);
		ActiveAbility(DefaultAbility.AbilityHandle);
	}
	else
	{
		AssetID = SaveData.ID;
		SetNameV(SaveData.Name);
		SetRaceID(SaveData.RaceID);
		SetLevelV(SaveData.Level);

		SetActorLocation(SaveData.SpawnLocation);
		SetActorRotation(SaveData.SpawnRotation);

		const UDWCharacterData& CharacterData = GetCharacterData<UDWCharacterData>();
		if(CharacterData.IsValid())
		{
			FString contextString;
			
			if (CharacterData.AttackAbilityTable != nullptr)
			{
				TArray<FDWCharacterAttackAbilityData*> attackAbilities;
				CharacterData.AttackAbilityTable->GetAllRows(contextString, attackAbilities);
				for (int i = 0; i < attackAbilities.Num(); i++)
				{
					auto abilityData = *attackAbilities[i];
					abilityData.AbilityHandle = AcquireAbility(abilityData.AbilityClass, abilityData.AbilityLevel);
					AttackAbilities.Add(abilityData);
				}
			}

			if (CharacterData.SkillAbilityTable != nullptr)
			{
				TArray<FDWCharacterSkillAbilityData*> skillAbilities;
				CharacterData.SkillAbilityTable->GetAllRows(contextString, skillAbilities);
				for (int i = 0; i < skillAbilities.Num(); i++)
				{
					auto abilityData = *skillAbilities[i];
					abilityData.AbilityHandle = AcquireAbility(abilityData.AbilityClass, abilityData.AbilityLevel);
					SkillAbilities.Add(abilityData.AbilityID, abilityData);
				}
			}

			if (CharacterData.ActionAbilityTable != nullptr)
			{
				TArray<FDWCharacterActionAbilityData*> actionAbilities;
				CharacterData.ActionAbilityTable->GetAllRows(contextString, actionAbilities);
				for (int i = 0; i < actionAbilities.Num(); i++)
				{
					auto abilityData = *actionAbilities[i];
					abilityData.AbilityHandle = AcquireAbility(abilityData.AbilityClass, abilityData.AbilityLevel);
					ActionAbilities.Add(abilityData.ActionType, abilityData);
				}
			}

			if (CharacterData.FallingAttackAbility.AbilityClass != nullptr)
			{
				FallingAttackAbility = CharacterData.FallingAttackAbility;
				FallingAttackAbility.AbilityHandle = AcquireAbility(FallingAttackAbility.AbilityClass, FallingAttackAbility.AbilityLevel);
			}

			if(CharacterData.AbilityClass != nullptr)
			{
				DefaultAbility = FAbilityData();
				DefaultAbility.AbilityLevel = SaveData.Level;
				DefaultAbility.AbilityHandle = AcquireAbility(CharacterData.AbilityClass, DefaultAbility.AbilityLevel);
				ActiveAbility(DefaultAbility.AbilityHandle);
			}

			Nature = CharacterData.Nature;
			AttackDistance = CharacterData.AttackDistance;
			InteractDistance = CharacterData.InteractDistance;
			FollowDistance = CharacterData.FollowDistance;
			PatrolDistance = CharacterData.PatrolDistance;
			PatrolDuration = CharacterData.PatrolDuration;
			
			SaveData.InventoryData = CharacterData.InventoryData;
		}

		Inventory->LoadData(SaveData.InventoryData, this);
	}
}

FSaveData* ADWCharacter::ToData()
{
	static auto SaveData = FDWCharacterSaveData();

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

	return &SaveData;
}

void ADWCharacter::Death(IAbilityVitalityInterface* InKiller)
{
	Super::Death(InKiller);
}

void ADWCharacter::Revive(IAbilityVitalityInterface* InRescuer)
{
	Super::Revive(InRescuer);
}

bool ADWCharacter::CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction)
{
	switch (InInteractAction)
	{
		case EInteractAction::Revive:
		{
			if(!IsDead() && !IsDying())
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
	if (!IsInterrupting())
	{
		AbilitySystem->AddLooseGameplayTag(GetCharacterData<UDWCharacterData>().FreeToAnimTag);
	}
	if (bUnLockRotation)
	{
		AbilitySystem->RemoveLooseGameplayTag(GetCharacterData<UDWCharacterData>().LockRotationTag);
	}
}

void ADWCharacter::LimitToAnim(bool bInLockRotation /*= false*/, bool bUnSprint /*= false*/)
{
	if (!IsFlying() && !IsFalling() && !IsRiding() && !IsClimbing() && !IsDefending())
	{
		AbilitySystem->RemoveLooseGameplayTag(GetCharacterData<UDWCharacterData>().FreeToAnimTag);
	}
	if (bInLockRotation)
	{
		AbilitySystem->AddLooseGameplayTag(GetCharacterData<UDWCharacterData>().LockRotationTag);
	}
	if (bUnSprint)
	{
		UnSprint();
	}
}

void ADWCharacter::Interrupt(float InDuration /*= -1*/)
{
	FSM->GetStateByClass<UDWCharacterState_Interrupt>()->Duration = InDuration;
	FSM->SwitchStateByClass<UDWCharacterState_Interrupt>();
}

void ADWCharacter::Jump()
{
	Super::Jump();
}

void ADWCharacter::UnJump()
{
	Super::UnJump();
}

void ADWCharacter::Dodge()
{
	FSM->SwitchStateByClass<UDWCharacterState_Dodge>();
}

void ADWCharacter::UnDodge()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Dodge>())
	{
		FSM->SwitchState(nullptr);
	}
}

void ADWCharacter::Sprint()
{
	if (!IsSprinting() && IsFreeToAnim())
	{
		AbilitySystem->AddLooseGameplayTag(GetCharacterData<UDWCharacterData>().SprintingTag);
	}
}

void ADWCharacter::UnSprint()
{
	if (IsSprinting())
	{
		AbilitySystem->RemoveLooseGameplayTag(GetCharacterData<UDWCharacterData>().SprintingTag);
	}
}

void ADWCharacter::Crouch(bool bClientSimulation)
{
	FSM->SwitchStateByClass<UDWCharacterState_Crouch>();
}

void ADWCharacter::UnCrouch()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Crouch>())
	{
		FSM->SwitchState(nullptr);
	}
}

void ADWCharacter::Swim()
{
	FSM->SwitchStateByClass<UDWCharacterState_Swim>();
}

void ADWCharacter::UnSwim()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Swim>())
	{
		FSM->SwitchState(nullptr);
	}
}

void ADWCharacter::Float(float InWaterPosZ)
{
	FSM->GetStateByClass<UDWCharacterState_Float>()->WaterPosZ = InWaterPosZ;
	FSM->SwitchStateByClass<UDWCharacterState_Float>();
}

void ADWCharacter::UnFloat()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Float>())
	{
		FSM->SwitchState(nullptr);
	}
}

void ADWCharacter::Climb()
{
	FSM->SwitchStateByClass<UDWCharacterState_Climb>();
}

void ADWCharacter::UnClimb()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Climb>())
	{
		FSM->SwitchState(nullptr);
	}
}

void ADWCharacter::Ride(ADWCharacter* InTarget)
{
	RidingTarget = InTarget;
	FSM->SwitchStateByClass<UDWCharacterState_Ride>();
}

void ADWCharacter::UnRide()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Ride>())
	{
		FSM->SwitchState(nullptr);
	}
}

void ADWCharacter::Fly()
{
	FSM->SwitchStateByClass<UDWCharacterState_Fly>();
}

void ADWCharacter::UnFly()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Fly>())
	{
		FSM->SwitchState(nullptr);
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
				FSM->SwitchStateByClass<UDWCharacterState_Attack>();
				AttackAbilityIndex = InAbilityIndex;
				AttackType = EDWCharacterAttackType::NormalAttack;
				NormalAttackRemainTime = 1.f / GetAttackSpeed();
				return true;
			}
			else
			{
				UnAttack();
			}
		}
	}
	else if(!IsAttacking() && IsFalling())
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
			switch(AbilityData.GetItemData<UAbilitySkillDataBase>().SkillMode)
			{
				case ESkillMode::Initiative:
				{
					if(AbilityData.WeaponType == EDWWeaponType::None || AbilityData.WeaponType == GetWeapon()->GetItemData<UDWEquipWeaponData>().WeaponType)
					{
						if(ActiveAbility(AbilityData.AbilityHandle))
						{
							FSM->SwitchStateByClass<UDWCharacterState_Attack>();
							SkillAbilityID = InSkillID;
							AttackType = EDWCharacterAttackType::SkillAttack;
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
			FSM->SwitchStateByClass<UDWCharacterState_Attack>();
			AttackType = EDWCharacterAttackType::FallingAttack;
			return true;
		}
	}
	return false;
}

void ADWCharacter::UnAttack()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Attack>())
	{
		FSM->SwitchState(nullptr);
	}
}

void ADWCharacter::Defend()
{
	FSM->SwitchStateByClass<UDWCharacterState_Defend>();
}

void ADWCharacter::UnDefend()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Defend>())
	{
		FSM->SwitchState(nullptr);
	}
}

bool ADWCharacter::UseItem(FAbilityItem& InItem)
{
	if(InItem.GetData().EqualType(EAbilityItemType::Voxel))
	{
		FVoxelHitResult voxelHitResult;
		if (RaycastVoxel(voxelHitResult))
		{
			return GenerateVoxel(static_cast<FVoxelItem&>(InItem), voxelHitResult);
		}
	}
	else if(InItem.GetData().EqualType(EAbilityItemType::Prop))
	{
		const UDWPropData& PropData = InItem.GetData<UDWPropData>();
		switch (PropData.PropType)
		{
			case EDWPropType::Potion:
			{
				if(PropData.Name.ToString().StartsWith(TEXT("生命")))
				{
					return GetHealth() < GetMaxHealth();
				}
				else if(PropData.Name.ToString().StartsWith(TEXT("魔法")))
				{
					return GetMana() < GetMaxMana();
				}
				else if(PropData.Name.ToString().StartsWith(TEXT("体力")))
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

void ADWCharacter::PickUp(AAbilityPickUpBase* InPickUp)
{
	if(InPickUp)
	{
		Inventory->AdditionItemByRange(InPickUp->GetItem(), -1);
	}
}

bool ADWCharacter::GenerateVoxel(FVoxelItem& InVoxelItem)
{
	bool bSuccess = false;
	FQueryItemInfo QueryItemInfo;
	if(!IsPlayer())
	{
		QueryItemInfo = Inventory->GetItemInfoByRange(EQueryItemType::Remove, static_cast<FAbilityItem>(InVoxelItem));
	}
	else if(Inventory->GetSelectedItem().EqualType(static_cast<FAbilityItem>(InVoxelItem)))
	{
		QueryItemInfo = FQueryItemInfo(Inventory->GetSelectedItem(), TArray<UInventorySlot*>{ Inventory->GetSelectedSlot() });
	}
	if(QueryItemInfo.Item.IsValid() && DoAction(EDWCharacterActionType::Generate))
	{
		AVoxelChunk* chunk = InVoxelItem.Owner;
		const FIndex index = InVoxelItem.Index;
		const FVoxelItem& voxelItem = chunk->GetVoxelItem(index);

		if(!voxelItem.IsValid() || voxelItem.GetData<UVoxelData>().Transparency == EVoxelTransparency::Transparent && voxelItem != InVoxelItem)
		{
			//FRotator rotation = (Owner->VoxelIndexToLocation(index) + tmpVoxel->GetData<UVoxelData>().GetCeilRange() * 0.5f * AVoxelModule::GetWorldInfo().BlockSize - UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>()->GetActorLocation()).ToOrientationRotator();
			//rotation = FRotator(FRotator::ClampAxis(FMath::RoundToInt(rotation.Pitch / 90) * 90.f), FRotator::ClampAxis(FMath::RoundToInt(rotation.Yaw / 90) * 90.f), FRotator::ClampAxis(FMath::RoundToInt(rotation.Roll / 90) * 90.f));
			//tmpVoxel->Rotation = rotation;

			FHitResult HitResult;
			if (!AMainModule::GetModuleByClass<AVoxelModule>()->VoxelTraceSingle(InVoxelItem, chunk->IndexToLocation(index), HitResult))
			{
				if(voxelItem.IsValid())
				{
					bSuccess = chunk->ReplaceVoxel(voxelItem, InVoxelItem);
				}
				else
				{
					bSuccess = chunk->GenerateVoxel(index, InVoxelItem);
				}
			}
		}
	}
	if(bSuccess) Inventory->RemoveItemBySlots(QueryItemInfo.Item, QueryItemInfo.Slots);
	return bSuccess;
}

bool ADWCharacter::GenerateVoxel(FVoxelItem& InVoxelItem, const FVoxelHitResult& InVoxelHitResult)
{
	bool bSuccess = false;
	FQueryItemInfo QueryItemInfo;
	if(!IsPlayer())
	{
		QueryItemInfo = Inventory->GetItemInfoByRange(EQueryItemType::Remove, static_cast<FAbilityItem>(InVoxelItem));
	}
	else if(Inventory->GetSelectedItem().EqualType(static_cast<FAbilityItem>(InVoxelItem)))
	{
		QueryItemInfo = FQueryItemInfo(Inventory->GetSelectedItem(), TArray<UInventorySlot*>{ Inventory->GetSelectedSlot() });
	}
	if(QueryItemInfo.Item.IsValid() && DoAction(EDWCharacterActionType::Generate))
	{
		AVoxelChunk* chunk = InVoxelHitResult.GetOwner();
		const FIndex index = chunk->LocationToIndex(InVoxelHitResult.Point - AVoxelModule::GetWorldData()->GetBlockSizedNormal(InVoxelHitResult.Normal)) + FIndex(InVoxelHitResult.Normal);
		const FVoxelItem& voxelItem = chunk->GetVoxelItem(index);

		if(!voxelItem.IsValid() || voxelItem.GetData<UVoxelData>().Transparency == EVoxelTransparency::Transparent && voxelItem != InVoxelHitResult.VoxelItem)
		{
			//FRotator rotation = (Owner->VoxelIndexToLocation(index) + tmpVoxel->GetData<UVoxelData>().GetCeilRange() * 0.5f * AVoxelModule::GetWorldInfo().BlockSize - UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>()->GetActorLocation()).ToOrientationRotator();
			//rotation = FRotator(FRotator::ClampAxis(FMath::RoundToInt(rotation.Pitch / 90) * 90.f), FRotator::ClampAxis(FMath::RoundToInt(rotation.Yaw / 90) * 90.f), FRotator::ClampAxis(FMath::RoundToInt(rotation.Roll / 90) * 90.f));
			//tmpVoxel->Rotation = rotation;

			FHitResult HitResult;
			if (!AMainModule::GetModuleByClass<AVoxelModule>()->VoxelTraceSingle(InVoxelItem, chunk->IndexToLocation(index), HitResult))
			{
				if(voxelItem.IsValid())
				{
					bSuccess = chunk->ReplaceVoxel(voxelItem, InVoxelItem);
				}
				else
				{
					bSuccess = chunk->GenerateVoxel(index, InVoxelItem);
				}
			}
		}
	}
	if(bSuccess) Inventory->RemoveItemBySlots(QueryItemInfo.Item, QueryItemInfo.Slots);
	return bSuccess;
}

bool ADWCharacter::DestroyVoxel(FVoxelItem& InVoxelItem)
{
	if(DoAction(EDWCharacterActionType::Destroy))
	{
		Super::DestroyVoxel(InVoxelItem);
	}
	return false;
}

bool ADWCharacter::DestroyVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(DoAction(EDWCharacterActionType::Destroy))
	{
		Super::DestroyVoxel(InVoxelHitResult);
	}
	return false;
}

void ADWCharacter::RefreshEquip(EDWEquipPartType InPartType, UInventoryEquipSlot* EquipSlot)
{
	if (!EquipSlot->IsEmpty())
	{
		FActorSpawnParameters spawnParams = FActorSpawnParameters();
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Equips[InPartType] = GetWorld()->SpawnActor<AAbilityEquipBase>(EquipSlot->GetItem().GetData<UAbilityEquipDataBase>().EquipClass, spawnParams);
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
		if (StopAction(ActionType, true) && ActiveAbility(AbilityData.AbilityHandle))
		{
			ActionType = InActionType;
			return true;
		}
	}
	return false;
}

bool ADWCharacter::StopAction(EDWCharacterActionType InActionType, bool bCancelAbility)
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
	if(InTargetCharacter && !IsDodging())
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
	if (IsBreakAllInput()) return;

	switch(GetCharacterMovement()->MovementMode)
	{
		case EMovementMode::MOVE_Swimming:
		{
			if(IsFloating() && WorldDirection.Z > -0.5f)
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

void ADWCharacter::SetDamageAble(bool bInDamaging)
{
	
}

UInventory* ADWCharacter::GetInventory() const
{
	return Inventory;
}

bool ADWCharacter::HasBehaviorTree() const
{
	return GetCharacterData<UDWCharacterData>().BehaviorTreeAsset != nullptr;
}

UBehaviorTree* ADWCharacter::GetBehaviorTree()
{
	const UDWCharacterData& CharacterData = GetCharacterData<UDWCharacterData>();
	if (!BehaviorTree)
	{
		BehaviorTree = DuplicateObject<UBehaviorTree>(CharacterData.BehaviorTreeAsset, this);
		if(BehaviorTree)
		{
			UBlackboardData* Blackboard = DuplicateObject<UBlackboardData>(CharacterData.BehaviorTreeAsset->BlackboardAsset, nullptr);
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

bool ADWCharacter::IsActive(bool bNeedNotDead, bool bNeedFreeToAnim /*= false*/) const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().StaticTag) && (!bNeedNotDead || !IsDead()) && (!bNeedFreeToAnim || IsFreeToAnim(false));
}

bool ADWCharacter::IsFreeToAnim(bool bCheckStates) const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().FreeToAnimTag) && (!bCheckStates || !IsFlying() && !IsFalling() && !IsRiding() && !IsClimbing() && !IsDefending());
}

bool ADWCharacter::IsFalling() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().FallingTag);
}

bool ADWCharacter::IsDodging() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().DodgingTag);
}

bool ADWCharacter::IsSprinting() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().SprintingTag);
}

bool ADWCharacter::IsCrouching() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().CrouchingTag);
}

bool ADWCharacter::IsSwimming() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().SwimmingTag);
}

bool ADWCharacter::IsFloating() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().FloatingTag);
}

bool ADWCharacter::IsAttacking() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().AttackingTag);
}

bool ADWCharacter::IsDefending() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().DefendingTag);
}

bool ADWCharacter::IsClimbing() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().ClimbingTag);
}

bool ADWCharacter::IsRiding() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().RidingTag);
}

bool ADWCharacter::IsFlying() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().FlyingTag);
}

bool ADWCharacter::IsInterrupting() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().InterruptingTag);
}

bool ADWCharacter::IsLockRotation() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().LockRotationTag);
}

bool ADWCharacter::IsBreakAllInput() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().BreakAllInputTag);
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

void ADWCharacter::SetNameV(FName InName)
{
	Super::SetNameV(InName);

	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::SetRaceID(FName InRaceID)
{
	Super::SetRaceID(InRaceID);
	
	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::SetLevelV(int32 InLevel)
{
	Super::SetLevelV(InLevel);

	FallingAttackAbility.AbilityLevel = InLevel;

	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::SetEXP(int32 InEXP)
{
	Super::SetEXP(InEXP);

	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::SetTeamID(FName InTeamID)
{
	TeamID = InTeamID;

	if (GetWidgetCharacterHPWidget())
	{
		GetWidgetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
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

bool ADWCharacter::HasWeapon(EDWWeaponType InWeaponType)
{
	if(InWeaponType == EDWWeaponType::None) return true;
	
	if(HasEquip(EDWEquipPartType::RightHand))
	{
		if(ADWEquipWeapon* Weapon = Cast<ADWEquipWeapon>(GetEquip(EDWEquipPartType::RightHand)))
		{
			return Weapon->GetItemData<UDWEquipWeaponData>().WeaponType == InWeaponType;
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
			return Weapon->GetItemData<UDWEquipShieldData>().ShieldType == InShieldType;
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
			if(Iter.Value.GetItemData<UAbilitySkillDataBase>().SkillType == InSkillType)
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
		if(Iter.Value.GetItemData<UAbilitySkillDataBase>().SkillType == InSkillType)
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

void ADWCharacter::OnInventorySlotSelected(UInventorySlot* InInventorySlot)
{
	const FAbilityItem tmpItem = InInventorySlot->GetItem();
	if(tmpItem.IsValid() && tmpItem.GetData().EqualType(EAbilityItemType::Voxel))
	{
		GeneratingVoxelItem = tmpItem.ID;
	}
}

void ADWCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	Super::OnAttributeChange(InAttributeChangeData);
	
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;
	if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetHealthAttribute())
	{
		if (GetWidgetCharacterHPWidget())
		{
			GetWidgetCharacterHPWidget()->SetHealthPercent(InAttributeChangeData.NewValue, GetMaxHealth());
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMaxHealthAttribute())
	{
		if (GetWidgetCharacterHPWidget())
		{
			GetWidgetCharacterHPWidget()->SetHealthPercent(GetHealth(), InAttributeChangeData.NewValue);
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetManaAttribute())
	{
		if (GetWidgetCharacterHPWidget())
		{
			GetWidgetCharacterHPWidget()->SetManaPercent(InAttributeChangeData.NewValue, GetMaxMana());
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMaxManaAttribute())
	{
		if (GetWidgetCharacterHPWidget())
		{
			GetWidgetCharacterHPWidget()->SetManaPercent(GetMana(), InAttributeChangeData.NewValue);
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMaxStaminaAttribute())
	{
		if (GetWidgetCharacterHPWidget())
		{
			GetWidgetCharacterHPWidget()->SetStaminaPercent(InAttributeChangeData.NewValue, GetMaxStamina());
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMaxStaminaAttribute())
	{
		if (GetWidgetCharacterHPWidget())
		{
			GetWidgetCharacterHPWidget()->SetStaminaPercent(GetStamina(), InAttributeChangeData.NewValue);
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMoveSpeedAttribute())
	{
		GetCharacterMovement()->MaxWalkSpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1) * MovementRate;
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetSwimSpeedAttribute())
	{
		GetCharacterMovement()->MaxSwimSpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1) * MovementRate;
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetRideSpeedAttribute())
	{
		if(RidingTarget)
		{
			RidingTarget->GetCharacterMovement()->MaxWalkSpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1) * MovementRate;
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetFlySpeedAttribute())
	{
		GetCharacterMovement()->MaxFlySpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1) * MovementRate;
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetRotationSpeedAttribute())
	{
		GetCharacterMovement()->RotationRate = FRotator(0, InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1), 0) * RotationRate;
	}
}

void ADWCharacter::HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, IAbilityVitalityInterface* SourceVitality)
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
		Interrupt(InterruptDuration * (1 - GetToughnessRate()));
	}
}
