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
#include "AI/DWAIBlackboard.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Character/CharacterModuleBPLibrary.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Character/States/DWCharacterState_Climb.h"
#include "Character/States/DWCharacterState_Crouch.h"
#include "Character/States/DWCharacterState_Death.h"
#include "Character/States/DWCharacterState_Default.h"
#include "Character/States/DWCharacterState_Defend.h"
#include "Character/States/DWCharacterState_Dodge.h"
#include "Character/States/DWCharacterState_Fall.h"
#include "Character/States/DWCharacterState_Float.h"
#include "Character/States/DWCharacterState_Fly.h"
#include "Character/States/DWCharacterState_Interrupt.h"
#include "Character/States/DWCharacterState_Jump.h"
#include "Character/States/DWCharacterState_Ride.h"
#include "Character/States/DWCharacterState_Static.h"
#include "Character/States/DWCharacterState_Swim.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "FSM/Components/FSMComponent.h"
#include "Inventory/Slot/InventoryEquipSlot.h"
#include "Inventory/Slot/InventorySkillSlot.h"
#include "Item/Prop/DWPropData.h"
#include "Main/MainModuleBPLibrary.h"
#include "Team/DWTeamModule.h"
#include "Voxel/Datas/VoxelData.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Widget/WidgetItemInfoBox.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/World/WorldWidgetComponent.h"

ADWCharacter::ADWCharacter()
{
	CharacterHP = CreateDefaultSubobject<UWorldWidgetComponent>(FName("CharacterHP"));
	CharacterHP->SetupAttachment(RootComponent);
	CharacterHP->SetRelativeLocation(FVector(0, 0, 90));
	CharacterHP->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UWidgetCharacterHP> CharacterHPClassFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/World/WB_CharacterHP.WB_CharacterHP_C'"));
	if(CharacterHPClassFinder.Succeeded())
	{
		CharacterHP->SetWorldWidgetClass(CharacterHPClassFinder.Class);
	}

	AbilitySystem = CreateDefaultSubobject<UDWAbilitySystemComponent>(FName("AbilitySystem"));

	AttributeSet = CreateDefaultSubobject<UDWCharacterAttributeSet>(FName("AttributeSet"));

	Inventory = CreateDefaultSubobject<UCharacterInventory>(FName("Inventory"));

	FSM->DefaultState = UDWCharacterState_Default::StaticClass();
	FSM->States.Empty();
	FSM->States.Add(UDWCharacterState_Attack::StaticClass());
	FSM->States.Add(UDWCharacterState_Climb::StaticClass());
	FSM->States.Add(UDWCharacterState_Crouch::StaticClass());
	FSM->States.Add(UDWCharacterState_Death::StaticClass());
	FSM->States.Add(UDWCharacterState_Default::StaticClass());
	FSM->States.Add(UDWCharacterState_Defend::StaticClass());
	FSM->States.Add(UDWCharacterState_Dodge::StaticClass());
	FSM->States.Add(UDWCharacterState_Fall::StaticClass());
	FSM->States.Add(UDWCharacterState_Float::StaticClass());
	FSM->States.Add(UDWCharacterState_Fly::StaticClass());
	FSM->States.Add(UDWCharacterState_Interrupt::StaticClass());
	FSM->States.Add(UDWCharacterState_Jump::StaticClass());
	FSM->States.Add(UDWCharacterState_Ride::StaticClass());
	FSM->States.Add(UDWCharacterState_Static::StaticClass());
	FSM->States.Add(UDWCharacterState_Swim::StaticClass());
	FSM->States.Add(UDWCharacterState_Walk::StaticClass());

	// stats
	ControlMode = EDWCharacterControlMode::Fighting;
	TeamID = NAME_None;
	
	Equips = TMap<EDWEquipPartType, AAbilityEquipBase*>();

	RidingTarget = nullptr;
	LockedTarget = nullptr;

	// local
	AttackAbilityIndex = 0;
	SkillAbilityID = FPrimaryAssetId();
	AttackType = EDWCharacterAttackType::None;
	BirthLocation = FVector(0, 0, 0);
	AIMoveLocation = Vector_Empty;
	AIMoveStopDistance = 0;

	DefaultAbility = FAbilityData();
	AttackAbilities = TMap<EDWWeaponType, FDWCharacterAttackAbilityDatas>();
	SkillAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData>();
	ActionAbilities = TMap<EDWCharacterActionType, FDWCharacterActionAbilityData>();

	AIControllerClass = ADWAIController::StaticClass();
}

// Called when the game starts or when spawned
void ADWCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADWCharacter::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void ADWCharacter::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();

	Inventory->UnloadSaveData(true);
	
	for(auto& Iter : Equips)
	{
		UObjectPoolModuleBPLibrary::DespawnObject(Iter.Value);
	}
	Equips.Empty();
}

void ADWCharacter::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	Super::LoadData(InSaveData, bForceMode);
	
	auto& SaveData = InSaveData->CastRef<FDWCharacterSaveData>();
	
	if(bForceMode)
	{
		SetControlMode(SaveData.ControlMode);
		SetTeamID(SaveData.TeamID);

		if(SaveData.IsSaved())
		{
			DefaultAbility = SaveData.DefaultAbility;
			FallingAttackAbility = SaveData.FallingAttackAbility;
			AttackAbilities = SaveData.AttackAbilities;
			SkillAbilities = SaveData.SkillAbilities;
			ActionAbilities = SaveData.ActionAbilities;

			for(auto& Iter : AttackAbilities)
			{
				for(auto& Iter1 : Iter.Value.Array)
				{
					Iter1.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter1.AbilityClass, Iter1.AbilityLevel);
				}
			}
			
			for(auto& Iter : SkillAbilities)
			{
				Iter.Value.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.Value.AbilityClass, Iter.Value.AbilityLevel);
			}

			for(auto& Iter : ActionAbilities)
			{
				Iter.Value.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.Value.AbilityClass, Iter.Value.AbilityLevel);
			}

			if (FallingAttackAbility.AbilityClass)
			{
				FallingAttackAbility.AbilityHandle = AbilitySystem->K2_GiveAbility(FallingAttackAbility.AbilityClass, FallingAttackAbility.AbilityLevel);
			}

			if(GetCharacterData<UAbilityCharacterDataBase>().AbilityClass)
			{
				DefaultAbility.AbilityHandle = AbilitySystem->K2_GiveAbility(GetCharacterData<UAbilityCharacterDataBase>().AbilityClass, DefaultAbility.AbilityLevel);
			}
		}
		else
		{
			BirthLocation = GetActorLocation();

			const UDWCharacterData& CharacterData = GetCharacterData<UDWCharacterData>();
			if(CharacterData.IsValid())
			{
				TArray<FDWCharacterAttackAbilityData> attackAbilities;
				UAssetModuleBPLibrary::ReadDataTable(CharacterData.AttackAbilityTable, attackAbilities);
				for(auto Iter : attackAbilities)
				{
					Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.AbilityLevel);
					if(!AttackAbilities.Contains(Iter.WeaponType)) AttackAbilities.Add(Iter.WeaponType);
					AttackAbilities[Iter.WeaponType].Array.Add(Iter);
				}

				TArray<FDWCharacterSkillAbilityData> skillAbilities;
				UAssetModuleBPLibrary::ReadDataTable(CharacterData.SkillAbilityTable, skillAbilities);
				for(auto Iter : skillAbilities)
				{
					Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.AbilityLevel);
					SkillAbilities.Add(Iter.AbilityID, Iter);
				}

				TArray<FDWCharacterActionAbilityData> actionAbilities;
				UAssetModuleBPLibrary::ReadDataTable(CharacterData.ActionAbilityTable, actionAbilities);
				for(auto Iter : actionAbilities)
				{
					Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.AbilityLevel);
					ActionAbilities.Add(Iter.ActionType, Iter);
				}

				if (CharacterData.FallingAttackAbility.AbilityClass)
				{
					FallingAttackAbility = CharacterData.FallingAttackAbility;
					FallingAttackAbility.AbilityHandle = AbilitySystem->K2_GiveAbility(FallingAttackAbility.AbilityClass, FallingAttackAbility.AbilityLevel);
				}

				if(CharacterData.AbilityClass)
				{
					DefaultAbility = FAbilityData();
					DefaultAbility.AbilityLevel = SaveData.Level;
					DefaultAbility.AbilityHandle = AbilitySystem->K2_GiveAbility(CharacterData.AbilityClass, DefaultAbility.AbilityLevel);
				}
				
				SaveData.InventoryData = CharacterData.InventoryData;

				if(!IsPlayer())
				{
					auto EquipDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilityEquipDataBase>(UAbilityModuleBPLibrary::ItemTypeToAssetType(EAbilityItemType::Equip));
					const int32 EquipNum = FMath::Clamp(FMath::Rand() < 0.2f ? FMath::RandRange(1, 3) : 0, 0, EquipDatas.Num());
					for (int32 i = 0; i < EquipNum; i++)
					{
						FAbilityItem tmpItem = FAbilityItem(EquipDatas[FMath::RandRange(0, EquipDatas.Num() - 1)]->GetPrimaryAssetId(), 1);
						SaveData.InventoryData.AddItem(tmpItem);
					}
				}
			}
		}
		if(DefaultAbility.IsValid())
		{
			AbilitySystem->TryActivateAbility(DefaultAbility.AbilityHandle);
		}

		Inventory->SetOwnerActor(this);
	}
	
	Inventory->LoadSaveData(&SaveData.InventoryData, bForceMode);
}

FSaveData* ADWCharacter::ToData()
{
	static FDWCharacterSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FCharacterSaveData>();

	SaveData.TeamID = TeamID;
	SaveData.ControlMode = ControlMode;

	SaveData.InventoryData = Inventory->ToSaveDataRef<FInventorySaveData>();

	SaveData.DefaultAbility = DefaultAbility;
	SaveData.FallingAttackAbility = FallingAttackAbility;
	SaveData.AttackAbilities = AttackAbilities;
	SaveData.SkillAbilities = SkillAbilities;
	SaveData.ActionAbilities = ActionAbilities;

	return &SaveData;
}

void ADWCharacter::SetActorVisible_Implementation(bool bNewVisible)
{
	Super::SetActorVisible_Implementation(bNewVisible);

	if(bNewVisible)
	{
		SetControlMode(ControlMode);
	}
}

void ADWCharacter::RefreshState()
{
	Super::RefreshState();

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

// Called every frame
void ADWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;

	if (IsActive())
	{
		if (LockedTarget)
		{
			if (!LockedTarget->IsDead() && FVector::Distance(GetActorLocation(), LockedTarget->GetActorLocation()) <= 1000.f)
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

		if(!IsPlayer())
		{
			if(AVoxelChunk* Chunk = UVoxelModuleBPLibrary::FindChunkByLocation(GetActorLocation()))
			{
				Chunk->AddSceneActor(this);
			}
			else if(Container)
			{
				Container->RemoveSceneActor(this);
			}
		}

		if(IsSprinting() && GetMoveDirection().Size() > 0.2f)
		{
			ModifyStamina(ATTRIBUTE_DELTAVALUE_CLAMP(Stamina, -GetStaminaExpendSpeed() * DeltaTime));
		}
		else if(IsFreeToAnim())
		{
			ModifyStamina(ATTRIBUTE_DELTAVALUE_CLAMP(Stamina, GetStaminaRegenSpeed() * DeltaTime));
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
			if(IsDead())
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
	Super::OnInteract(InInteractionAgent, InInteractAction);
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
	if (!IsFreeToAnim()/* && !IsFlying() && !IsFalling() && !IsRiding() && !IsClimbing() && !IsDodging() && !IsDefending() && !IsInterrupting()*/)
	{
		AbilitySystem->AddLooseGameplayTag(GetCharacterData<UDWCharacterData>().FreeToAnimTag);
	}
	if (bUnLockRotation && IsLockRotation())
	{
		AbilitySystem->RemoveLooseGameplayTag(GetCharacterData<UDWCharacterData>().LockRotationTag);
	}
}

void ADWCharacter::LimitToAnim(bool bLockRotation /*= false*/, bool bUnSprint /*= false*/)
{
	if (IsFreeToAnim())
	{
		AbilitySystem->RemoveLooseGameplayTag(GetCharacterData<UDWCharacterData>().FreeToAnimTag);
	}
	if (bLockRotation && !IsLockRotation())
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

void ADWCharacter::UnInterrupt()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Interrupt>())
	{
		FSM->SwitchState(nullptr);
	}
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
	if (!IsSprinting())
	{
		DoAction(EDWCharacterActionType::Sprint);
	}
}

void ADWCharacter::UnSprint()
{
	if (IsSprinting())
	{
		StopAction(EDWCharacterActionType::Sprint);
	}
}

void ADWCharacter::Crouch(bool bClientSimulation)
{
	if(!FSM->IsCurrentStateClass<UDWCharacterState_Crouch>())
	{
		FSM->SwitchStateByClass<UDWCharacterState_Crouch>();
	}
	else
	{
		Super::Crouch(bClientSimulation);
	}
}

void ADWCharacter::UnCrouch(bool bClientSimulation)
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Crouch>())
	{
		FSM->SwitchState(nullptr);
	}
	else
	{
		Super::UnCrouch(bClientSimulation);
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
	if(InAbilityIndex == -1) InAbilityIndex = AttackAbilityIndex;
	
	if(!IsFalling())
	{
		if (HasAttackAbility(InAbilityIndex))
		{
			if (AbilitySystem->TryActivateAbility(GetAttackAbility(InAbilityIndex).AbilityHandle))
			{
				AttackAbilityIndex = InAbilityIndex;
				AttackType = EDWCharacterAttackType::NormalAttack;
				FSM->SwitchStateByClass<UDWCharacterState_Attack>();
				return true;
			}
		}
	}
	else
	{
		return FallingAttack();
	}
	return false;
}

bool ADWCharacter::SkillAttack(const FPrimaryAssetId& InSkillID)
{
	if (HasSkillAbility(InSkillID, true))
	{
		const auto AbilityData = GetSkillAbility(InSkillID);
		switch(AbilityData.GetItemData<UAbilitySkillDataBase>().SkillMode)
		{
			case ESkillMode::Initiative:
			{
				if(CheckWeaponType(AbilityData.WeaponType))
				{
					if(AbilitySystem->TryActivateAbility(AbilityData.AbilityHandle))
					{
						SkillAbilityID = InSkillID;
						AttackType = EDWCharacterAttackType::SkillAttack;
						FSM->SwitchStateByClass<UDWCharacterState_Attack>();
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
	return false;
}

bool ADWCharacter::SkillAttack(ESkillType InSkillType, int32 InAbilityIndex)
{
	if (HasSkillAbility(InSkillType, InAbilityIndex, true))
	{
		const auto AbilityData = GetSkillAbility(InSkillType, InAbilityIndex);
		SkillAttack(AbilityData.AbilityID);
	}
	return false;
}

bool ADWCharacter::FallingAttack()
{
	if(FallingAttackAbility.IsValid())
	{
		if(CheckWeaponType(FallingAttackAbility.WeaponType))
		{
			if (AbilitySystem->TryActivateAbility(FallingAttackAbility.AbilityHandle))
			{
				AttackType = EDWCharacterAttackType::FallingAttack;
				FSM->SwitchStateByClass<UDWCharacterState_Attack>();
				return true;
			}
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
	switch (InItem.GetData().GetItemType())
	{
		case EAbilityItemType::Prop:
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
			break;
		}
		default: break;
	}
	return false;
}

void ADWCharacter::PickUp(AAbilityPickUpBase* InPickUp)
{
	if(InPickUp)
	{
		Inventory->AddItemByRange(InPickUp->GetItem(), -1);
	}
}

bool ADWCharacter::GenerateVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(!GenerateVoxelID.IsValid()) return false;
	
	FQueryItemInfo QueryItemInfo = Inventory->QueryItemByRange(EQueryItemType::Remove, FAbilityItem(GenerateVoxelID, 1), -1);
	if(QueryItemInfo.IsSuccess() && DoAction(EDWCharacterActionType::Generate))
	{
		if(Super::GenerateVoxel(InVoxelHitResult))
		{
			Inventory->RemoveItemByQueryInfo(QueryItemInfo);
			return true;
		}
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

void ADWCharacter::RefreshEquip(EDWEquipPartType InPartType, const FAbilityItem& InItem)
{
	AAbilityEquipBase* Equip = GetEquip(InPartType);
	if (InItem.IsValid())
	{
		if(Equip && !Equip->GetItemData().EqualID(InItem.ID))
		{
			Equip->OnDischarge();
			UObjectPoolModuleBPLibrary::DespawnObject(Equip);
			Equip = nullptr;
		}
		if(!Equip)
		{
			Equip = UObjectPoolModuleBPLibrary::SpawnObject<AAbilityEquipBase>(nullptr, InItem.GetData<UAbilityEquipDataBase>().EquipClass);
			Equip->Initialize(this, InItem);
			Equip->OnAssemble();
			Equip->Execute_SetActorVisible(Equip, Execute_IsVisible(this) && ControlMode == EDWCharacterControlMode::Fighting);
			Equips.Emplace(InPartType, Equip);
		}
	}
	else if(Equip)
	{
		Equip->OnDischarge();
		UObjectPoolModuleBPLibrary::DespawnObject(Equip);
		Equips.Remove(InPartType);
	}
}

bool ADWCharacter::DoAction(EDWCharacterActionType InActionType)
{
	if (!HasActionAbility(InActionType)) return false;

	const FDWCharacterActionAbilityData AbilityData = GetActionAbility(InActionType);
	bool bSuccess = AbilitySystem->TryActivateAbility(AbilityData.AbilityHandle);
	const FGameplayAbilitySpec Spec = AbilitySystem->FindAbilitySpecForHandle(AbilityData.AbilityHandle);
	if(UDWCharacterActionAbility* Ability = Cast<UDWCharacterActionAbility>(Spec.GetPrimaryInstance()))
	{
		Ability->SetActionType(InActionType);
		Ability->SetStopped(false);
	}
	return bSuccess;
}

bool ADWCharacter::StopAction(EDWCharacterActionType InActionType)
{
	if (!HasActionAbility(InActionType)) return false;

	const FDWCharacterActionAbilityData AbilityData = GetActionAbility(InActionType);
	const FGameplayAbilitySpec Spec = AbilitySystem->FindAbilitySpecForHandle(AbilityData.AbilityHandle);
	if(UDWCharacterActionAbility* Ability = Cast<UDWCharacterActionAbility>(Spec.GetPrimaryInstance()))
	{
		Ability->SetStopped(true);
	}
	AbilitySystem->CancelAbilityHandle(AbilityData.AbilityHandle);
	return true;
}

void ADWCharacter::EndAction(EDWCharacterActionType InActionType, bool bWasCancelled)
{
	if (!HasActionAbility(InActionType)) return;

	switch(InActionType)
	{
		case EDWCharacterActionType::Death:
		{
			FSM->GetCurrentState<UDWCharacterState_Death>()->DeathEnd();
			break;
		}
		case EDWCharacterActionType::Crouch:
		{
			UnCrouch(false);
			break;
		}
		case EDWCharacterActionType::Dodge:
		{
			UnDodge();
			break;
		}
		case EDWCharacterActionType::Sprint:
		{
			UnSprint();
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
		case EDWCharacterActionType::Float:
		{
			UnFloat();
			break;
		}
		case EDWCharacterActionType::Ride:
		{
			UnRide();
			break;
		}
		case EDWCharacterActionType::Fly:
		{
			UnFly();
			break;
		}
		case EDWCharacterActionType::Attack:
		{
			UnAttack();
			break;
		}
		case EDWCharacterActionType::Defend:
		{
			UnDefend();
			break;
		}
		default: break;
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

	if (GetDistance(InTargetCharacter, false, false) > InMoveStopDistance)
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

void ADWCharacter::SetAttackDamageAble(bool bInDamaging)
{
	
}

UWidgetCharacterHP* ADWCharacter::GetCharacterHPWidget() const
{
	if (CharacterHP->GetWorldWidget())
	{
		return Cast<UWidgetCharacterHP>(CharacterHP->GetWorldWidget());
	}
	return nullptr;
}

UInventory* ADWCharacter::GetInventory() const
{
	return Inventory;
}

bool ADWCharacter::IsExhausted() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().ExhaustedTag);
}

bool ADWCharacter::IsFreeToAnim() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().FreeToAnimTag);
}

bool ADWCharacter::IsDodging() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().DodgingTag);
}

bool ADWCharacter::IsSprinting() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().SprintingTag);
}

bool ADWCharacter::IsCrouching(bool bMovementMode) const
{
	return !bMovementMode ? AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().CrouchingTag) : GetCharacterMovement()->IsFlying();
}

bool ADWCharacter::IsSwimming(bool bMovementMode) const
{
	return !bMovementMode ? AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().SwimmingTag) : GetCharacterMovement()->IsFlying();
}

bool ADWCharacter::IsFloating() const
{
	return AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().FloatingTag);
}

bool ADWCharacter::IsAttacking(bool bAttackType) const
{
	return !bAttackType ? AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().AttackingTag) : AttackType != EDWCharacterAttackType::None;
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

bool ADWCharacter::IsFlying(bool bMovementMode) const
{
	return !bMovementMode ? AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().FlyingTag) : GetCharacterMovement()->IsFlying();
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

void ADWCharacter::SetControlMode(EDWCharacterControlMode InControlMode)
{
	ControlMode = InControlMode;

	if(!Execute_IsVisible(this)) return;

	switch (ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(GetWeapon()) GetWeapon()->Execute_SetActorVisible(GetWeapon(), true);
			if(GetShield()) GetShield()->Execute_SetActorVisible(GetShield(), true);
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			if(GetWeapon()) GetWeapon()->Execute_SetActorVisible(GetWeapon(), false);
			if(GetShield()) GetShield()->Execute_SetActorVisible(GetShield(), false);
			break;
		}
	}
}

FDWTeamData* ADWCharacter::GetTeamData() const
{
	if(ADWTeamModule* TeamModule = ADWTeamModule::Get())
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

	if (GetCharacterHPWidget())
	{
		GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::SetRaceID(FName InRaceID)
{
	Super::SetRaceID(InRaceID);
	
	if (GetCharacterHPWidget())
	{
		GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::SetLevelV(int32 InLevel)
{
	Super::SetLevelV(InLevel);

	FallingAttackAbility.AbilityLevel = InLevel;

	if (GetCharacterHPWidget())
	{
		GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::SetTeamID(FName InTeamID)
{
	TeamID = InTeamID;

	if (GetCharacterHPWidget())
	{
		GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

EDWCharacterNature ADWCharacter::GetNature() const
{
	return GetCharacterData<UDWCharacterData>().Nature;
}

float ADWCharacter::GetAttackDistance() const
{
	return GetCharacterData<UDWCharacterData>().AttackDistance;
}

float ADWCharacter::GetInteractDistance() const
{
	return GetCharacterData<UDWCharacterData>().InteractDistance;
}

float ADWCharacter::GetFollowDistance() const
{
	return GetCharacterData<UDWCharacterData>().FollowDistance;
}

float ADWCharacter::GetPatrolDistance() const
{
	return GetCharacterData<UDWCharacterData>().PatrolDistance;
}

float ADWCharacter::GetPatrolDuration() const
{
	return GetCharacterData<UDWCharacterData>().PatrolDuration;
}

AAbilityEquipBase* ADWCharacter::GetEquip(EDWEquipPartType InPartType) const
{
	if (Equips.Contains(InPartType))
	{
		return Equips[InPartType];
	}
	return nullptr;
}

ADWEquipWeapon* ADWCharacter::GetWeapon() const
{
	return GetEquip<ADWEquipWeapon>(EDWEquipPartType::RightHand);
}

ADWEquipShield* ADWCharacter::GetShield() const
{
	return GetEquip<ADWEquipShield>(EDWEquipPartType::LeftHand);
}

EDWWeaponType ADWCharacter::GetWeaponType() const
{
	if(GetWeapon())
	{
		return GetWeapon()->GetItemData<UDWEquipWeaponData>().WeaponType;
	}
	return EDWWeaponType::None;
}

bool ADWCharacter::CheckWeaponType(EDWWeaponType InWeaponType) const
{
	return InWeaponType == EDWWeaponType::None || GetWeaponType() == InWeaponType;
}

EDWShieldType ADWCharacter::GetShieldType() const
{
	if(GetShield())
	{
		return GetShield()->GetItemData<UDWEquipShieldData>().ShieldType;
	}
	return EDWShieldType::None;
}

bool ADWCharacter::CheckShieldType(EDWShieldType InShieldType) const
{
	return InShieldType == EDWShieldType::None || GetShieldType() == InShieldType;
}

FDWCharacterAttackAbilityData ADWCharacter::GetAttackAbility(int32 InAbilityIndex) const
{
	if(InAbilityIndex == -1) InAbilityIndex = AttackAbilityIndex;
	if(HasAttackAbility(InAbilityIndex))
	{
		return GetAttackAbilities()[InAbilityIndex];
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

FDWCharacterActionAbilityData ADWCharacter::GetActionAbility(EDWCharacterActionType InActionType)
{
	if (HasActionAbility(InActionType))
	{
		return ActionAbilities[InActionType];
	}
	return FDWCharacterActionAbilityData();
}

TArray<FDWCharacterAttackAbilityData> ADWCharacter::GetAttackAbilities() const
{
	if(AttackAbilities.Contains(GetWeaponType()))
	{
		return AttackAbilities[GetWeaponType()].Array;
	}
	return TArray<FDWCharacterAttackAbilityData>();
}

bool ADWCharacter::RaycastStep(FHitResult& OutHitResult)
{
	const FVector rayStart = GetActorLocation() + FVector::DownVector * (GetHalfHeight() - GetCharacterMovement()->MaxStepHeight);
	const FVector rayEnd = rayStart + GetMoveDirection() * (GetRadius() + AVoxelModule::Get()->GetWorldData().BlockSize * FMath::Clamp(GetMoveDirection().Size() * 0.005f, 0.5f, 1.3f));
	return UKismetSystemLibrary::LineTraceSingle(this, rayStart, rayEnd, UGlobalBPLibrary::GetGameTraceChannel((ECollisionChannel)EDWGameTraceType::Step), false, {}, EDrawDebugTrace::None, OutHitResult, true);
}

bool ADWCharacter::HasTeam() const
{
	return GetTeamData()->IsValid();
}

bool ADWCharacter::IsTeamMate(ADWCharacter* InTargetCharacter) const
{
	return HasTeam() && InTargetCharacter->TeamID == TeamID;
}

bool ADWCharacter::HasAttackAbility(int32 InAbilityIndex) const
{
	if(AttackAbilities.Contains(GetWeaponType()))
	{
		return AttackAbilities[GetWeaponType()].Array.IsValidIndex(InAbilityIndex);
	}
	return false;
}

bool ADWCharacter::HasSkillAbility(const FPrimaryAssetId& InSkillID, bool bNeedAssembled) const
{
	if(SkillAbilities.Contains(InSkillID))
	{
		if(!bNeedAssembled) return true;
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

bool ADWCharacter::HasSkillAbility(ESkillType InSkillType, int32 InAbilityIndex, bool bNeedAssembled) const
{
	TArray<FDWCharacterSkillAbilityData> Abilities = TArray<FDWCharacterSkillAbilityData>();
	for (auto Iter : SkillAbilities)
	{
		if(Iter.Value.GetItemData<UAbilitySkillDataBase>().SkillType == InSkillType)
		{
			Abilities.Add(Iter.Value);
		}
	}
	if(Abilities.IsValidIndex(InAbilityIndex))
	{
		if(!bNeedAssembled) return true;
		auto SkillSlots = Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill);
		for (int32 i = 0; i < SkillSlots.Num(); i++)
		{
			if(SkillSlots[i]->GetItem().ID == Abilities[InAbilityIndex].AbilityID)
			{
				return true;
			}
		}
	}
	return false;
}

bool ADWCharacter::HasActionAbility(EDWCharacterActionType InActionType) const
{
	if(InActionType == EDWCharacterActionType::None) return false;
	
	return ActionAbilities.Contains(InActionType);
}

bool ADWCharacter::CreateTeam(const FName& InTeamName /*= MANE_None*/, FString InTeamDetail /*= TEXT("")*/)
{
	if(ADWTeamModule* TeamModule = ADWTeamModule::Get())
	{
		return TeamModule->CreateTeam(this, InTeamName, InTeamDetail);
	}
	return false;
}

bool ADWCharacter::DissolveTeam()
{
	if(ADWTeamModule* TeamModule = ADWTeamModule::Get())
	{
		return TeamModule->DissolveTeam(TeamID, this);
	}
	return false;
}

bool ADWCharacter::JoinTeam(const FName& InTeamID)
{
	if(ADWTeamModule* TeamModule = ADWTeamModule::Get())
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
	return GetNature() == EDWCharacterNature::Player;
}

bool ADWCharacter::IsEnemy(ADWCharacter* InTargetCharacter) const
{
	switch (GetNature())
	{
		case EDWCharacterNature::NPC:
		case EDWCharacterNature::AIFriendly:
		{
			return false;
		}
		default:
		{
			switch (InTargetCharacter->GetNature())
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

UBehaviorTree* ADWCharacter::GetBehaviorTreeAsset() const
{
	return GetCharacterData<UDWCharacterData>().BehaviorTreeAsset;
}

void ADWCharacter::OnSelectedItemChange(const FAbilityItem& InItem)
{
	if(InItem.IsValid() && InItem.GetData().GetItemType() == EAbilityItemType::Voxel)
	{
		SetGenerateVoxelID(InItem.ID);
	}
	else
	{
		SetGenerateVoxelID(FPrimaryAssetId());
	}
}

void ADWCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	Super::OnAttributeChange(InAttributeChangeData);
	
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;

	if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetHealthAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxHealthAttribute())
	{
		if (GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetHealthPercent(GetHealth(), GetMaxHealth());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetExpAttribute())
	{
		if (GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetManaAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxManaAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetManaPercent(GetMana(), GetMaxMana());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
		if(!IsExhausted() && GetStamina() <= 0.f)
		{
			AbilitySystem->AddLooseGameplayTag(GetCharacterData<UDWCharacterData>().ExhaustedTag);
		}
		else if(IsExhausted())
		{
			AbilitySystem->RemoveLooseGameplayTag(GetCharacterData<UDWCharacterData>().ExhaustedTag);
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxStaminaAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMoveSpeedAttribute())
	{
		GetCharacterMovement()->MaxWalkSpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1) * MovementRate;
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetSwimSpeedAttribute())
	{
		GetCharacterMovement()->MaxSwimSpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1) * MovementRate;
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetRideSpeedAttribute())
	{
		if(RidingTarget)
		{
			RidingTarget->GetCharacterMovement()->MaxWalkSpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1) * MovementRate;
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetFlySpeedAttribute())
	{
		GetCharacterMovement()->MaxFlySpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1) * MovementRate;
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetRotationSpeedAttribute())
	{
		GetCharacterMovement()->RotationRate = FRotator(0, InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1), 0) * RotationRate;
	}
}

void ADWCharacter::HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, bool bHasDefend, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleDamage(DamageType, LocalDamageDone, bHasCrited, bHasDefend, HitResult, SourceTags, SourceActor);
	
	if(SourceActor && SourceActor != this)
	{
		if(ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor))
		{
			if(DamageType == EDamageType::Physics)
			{
				SourceCharacter->ModifyHealth(LocalDamageDone * SourceCharacter->GetAttackStealRate());
			}
			if(!IsPlayer() && !GetController<ADWAIController>()->GetBlackboard()->GetTargetCharacter())
			{
				GetController<ADWAIController>()->GetBlackboard()->SetTargetCharacter(SourceCharacter);
			}
			if(!bHasDefend)
			{
				DoAction(EDWCharacterActionType::GetHit);
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
