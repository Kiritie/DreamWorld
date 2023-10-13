// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Character/DWCharacter.h"

#include "Achievement/AchievementModuleBPLibrary.h"
#include "TimerManager.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Components/DWAbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "AI/DWAIController.h"
#include "Character/DWCharacterPart.h"
#include "Common/CommonBPLibrary.h"
#include "Scene/Actor/PhysicsVolume/PhysicsVolumeBase.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/VoxelModule.h"
#include "Widget/World/WidgetCharacterHP.h"
#include "Ability/Character/DWCharacterActionAbility.h"
#include "Ability/Character/DWCharacterAttackAbility.h"
#include "Ability/Character/DWCharacterAttributeSet.h"
#include "Ability/Character/DWCharacterSkillAbility.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Item/Equip/Shield/DWEquipShield.h"
#include "Item/Equip/Shield/DWEquipShieldData.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "Item/Equip/Weapon/DWEquipWeaponData.h"
#include "Ability/Item/Skill/AbilitySkillBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/PickUp/AbilityPickUpBase.h"
#include "AI/DWAIBlackboard.h"
#include "Asset/AssetModuleBPLibrary.h"
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
#include "Ability/Inventory/Slot/AbilityInventorySkillSlot.h"
#include "Gameplay/WHGameInstance.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Widget/Item/WidgetItemInfoBox.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/World/WorldWidgetComponent.h"
#include "Inventory/DWCharacterInventory.h"

// Sets default values
ADWCharacter::ADWCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UDWAbilitySystemComponent>("AbilitySystem").
		SetDefaultSubobjectClass<UDWCharacterAttributeSet>("AttributeSet").
		SetDefaultSubobjectClass<UDWCharacterInventory>("Inventory"))
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
	AttackAbilityQueue = 0;
	SkillAbilityID = FPrimaryAssetId();
	AttackType = EDWCharacterAttackType::None;
	BirthLocation = FVector(0, 0, 0);
	AIMoveLocation = Vector_Empty;
	AIMoveStopDistance = 0;

	AttackAbilities = TMap<EDWWeaponType, FDWCharacterAttackAbilityDatas>();
	SkillAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData>();
	ActionAbilities = TMap<EDWCharacterActionType, FDWCharacterActionAbilityData>();

	AIControllerClass = ADWAIController::StaticClass();
}

void ADWCharacter::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);

	if (IsDead()) return;

	if (IsActive())
	{
		if (LockedTarget)
		{
			if (CanLookAtTarget(LockedTarget))
			{
				DoLookAtTarget(LockedTarget);
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

		if(IsSprinting())
		{
			if(GetMoveDirection().Size() > 0.2f)
			{
				ModifyStamina(ATTRIBUTE_DELTAVALUE_CLAMP(Stamina, -GetStaminaExpendSpeed() * DeltaSeconds));
			}
			else
			{
				UnSprint();
			}
		}
		else if(IsFreeToAnim())
		{
			ModifyStamina(ATTRIBUTE_DELTAVALUE_CLAMP(Stamina, GetStaminaRegenSpeed() * DeltaSeconds));
		}

		if (GetActorLocation().Z < 0)
		{
			Death();
		}
	}
}

void ADWCharacter::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void ADWCharacter::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	for(auto& Iter : Equips)
	{
		UObjectPoolModuleBPLibrary::DespawnObject(Iter.Value);
	}
	Equips.Empty();
}

void ADWCharacter::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWCharacterSaveData>();

	if(PHASEC(InPhase, EPhase::PrimaryAndLesser))
	{
		if(!SaveData.InventoryData.IsSaved() && !IsPlayer())
		{
			SaveData.InventoryData = SaveData.GetItemData<UDWCharacterData>().InventoryData;

			auto EquipDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilityEquipDataBase>(FName("Equip"));
			const int32 EquipNum = FMath::Clamp(FMath::Rand() < 0.2f ? FMath::RandRange(1, 3) : 0, 0, EquipDatas.Num());
			for (int32 i = 0; i < EquipNum; i++)
			{
				FAbilityItem tmpItem = FAbilityItem(EquipDatas[FMath::RandRange(0, EquipDatas.Num() - 1)]->GetPrimaryAssetId(), 1);
				SaveData.InventoryData.AddItem(tmpItem, { ESlotSplitType::Default });
			}
		}
	}

	if(PHASEC(InPhase, EPhase::Primary))
	{
		SetControlMode(SaveData.ControlMode);
		SetTeamID(SaveData.TeamID);

		if(SaveData.IsSaved())
		{
			BirthLocation = SaveData.BirthLocation;
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
		}
		else
		{
			BirthLocation = SaveData.SpawnLocation;

			const UDWCharacterData& CharacterData = GetCharacterData<UDWCharacterData>();
			if(CharacterData.IsValid())
			{
				TArray<FDWCharacterAttackAbilityData> attackAbilities;
				UAssetModuleBPLibrary::ReadDataTable(CharacterData.AttackAbilityTable, attackAbilities);
				for(auto& Iter : attackAbilities)
				{
					Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.AbilityLevel);
					if(!AttackAbilities.Contains(Iter.WeaponType)) AttackAbilities.Add(Iter.WeaponType);
					AttackAbilities[Iter.WeaponType].Array.Add(Iter);
				}

				TArray<FDWCharacterSkillAbilityData> skillAbilities;
				UAssetModuleBPLibrary::ReadDataTable(CharacterData.SkillAbilityTable, skillAbilities);
				for(auto& Iter : skillAbilities)
				{
					Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.AbilityLevel);
					SkillAbilities.Add(Iter.AbilityID, Iter);
				}

				TArray<FDWCharacterActionAbilityData> actionAbilities;
				UAssetModuleBPLibrary::ReadDataTable(CharacterData.ActionAbilityTable, actionAbilities);
				for(auto& Iter : actionAbilities)
				{
					Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.AbilityLevel);
					ActionAbilities.Add(Iter.ActionType, Iter);
				}

				if (CharacterData.FallingAttackAbility.AbilityClass)
				{
					FallingAttackAbility = CharacterData.FallingAttackAbility;
					FallingAttackAbility.AbilityHandle = AbilitySystem->K2_GiveAbility(FallingAttackAbility.AbilityClass, FallingAttackAbility.AbilityLevel);
				}
			
			}
		}
	}

	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWCharacter::ToData(bool bRefresh)
{
	static FDWCharacterSaveData SaveData;
	SaveData = Super::ToData(bRefresh)->CastRef<FCharacterSaveData>();

	SaveData.TeamID = TeamID;
	SaveData.BirthLocation = BirthLocation;
	SaveData.ControlMode = ControlMode;

	SaveData.FallingAttackAbility = FallingAttackAbility;
	SaveData.AttackAbilities = AttackAbilities;
	SaveData.SkillAbilities = SkillAbilities;
	SaveData.ActionAbilities = ActionAbilities;

	return &SaveData;
}

void ADWCharacter::ResetData()
{
	Super::ResetData();

	SetMana(GetMaxMana());
	SetStamina(GetMaxStamina());
}

void ADWCharacter::SetActorVisible_Implementation(bool bInVisible)
{
	Super::SetActorVisible_Implementation(bInVisible);

	SetControlMode(ControlMode);
}

void ADWCharacter::RefreshState()
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
		default:
		{
			Super::RefreshState();
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

bool ADWCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	return Super::CanInteract(InInteractAction, InInteractionAgent);
}

void ADWCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassivity)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassivity);
	switch (InInteractAction)
	{
		case EInteractAction::Revive:
		{
			Revive();
		}
		default: break;
	}
}

void ADWCharacter::OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess)
{
	Super::OnActiveItem(InItem, bPassive, bSuccess);

	if(!bPassive)
	{
		if(bSuccess)
		{
			if(InItem.GetType() == EAbilityItemType::Prop)
			{
				DoAction(EDWCharacterActionType::Use);
			}
		}
		else if(IsPlayer())
		{
			UWidgetModuleBPLibrary::OpenUserWidget<UWidgetItemInfoBox>({ FString::Printf(TEXT("该%s还未准备好！"),
				*UCommonBPLibrary::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)InItem.GetType()).ToString()) });
		}
	}
}

void ADWCharacter::OnCancelItem(const FAbilityItem& InItem, bool bPassive)
{
	Super::OnCancelItem(InItem, bPassive);
}

void ADWCharacter::OnAssembleItem(const FAbilityItem& InItem)
{
	Super::OnAssembleItem(InItem);
}

void ADWCharacter::OnDischargeItem(const FAbilityItem& InItem)
{
	Super::OnDischargeItem(InItem);
}

void ADWCharacter::OnDiscardItem(const FAbilityItem& InItem, bool bInPlace)
{
	Super::OnDiscardItem(InItem, bInPlace);
}

void ADWCharacter::OnSelectItem(const FAbilityItem& InItem)
{
	Super::OnSelectItem(InItem);
}

void ADWCharacter::OnAuxiliaryItem(const FAbilityItem& InItem)
{
	Super::OnAuxiliaryItem(InItem);
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

void ADWCharacter::LimitToAnim(bool bLockRotation /*= false*/)
{
	if (IsFreeToAnim())
	{
		AbilitySystem->RemoveLooseGameplayTag(GetCharacterData<UDWCharacterData>().FreeToAnimTag);
	}
	if (bLockRotation && !IsLockRotation())
	{
		AbilitySystem->AddLooseGameplayTag(GetCharacterData<UDWCharacterData>().LockRotationTag);
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
	if (!IsSprinting() && GetStamina() > 0.f && GetMoveVelocity().Size() > 0.2f)
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

	if(!IsFalling(true))
	{
		if(HasAttackAbility(InAbilityIndex))
		{
			const auto AbilityData = GetAttackAbility(InAbilityIndex);
			if(CheckWeaponType(AbilityData.WeaponType))
			{
				if(FSM->SwitchStateByClass<UDWCharacterState_Attack>())
				{
					if (AbilitySystem->TryActivateAbility(AbilityData.AbilityHandle))
					{
						AttackType = EDWCharacterAttackType::NormalAttack;
						AttackAbilityIndex = InAbilityIndex;
						if(AttackAbilityQueue > 0)
						{
							AttackAbilityQueue--;
						}
						return true;
					}
				}
			}
		}
	}
	else if(FallingAttackAbility.IsValid())
	{
		if(CheckWeaponType(FallingAttackAbility.WeaponType))
		{
			if(FSM->SwitchStateByClass<UDWCharacterState_Attack>())
			{
				if(AbilitySystem->TryActivateAbility(FallingAttackAbility.AbilityHandle))
				{
					AttackType = EDWCharacterAttackType::FallingAttack;
					if(AttackAbilityQueue > 0)
					{
						AttackAbilityQueue--;
					}
					return true;
				}
			}
		}
	}
	AttackAbilityQueue = 0;
	return false;
}

bool ADWCharacter::SkillAttack(const FPrimaryAssetId& InSkillID)
{
	if(const auto SkillSlot = Inventory->GetSlotBySplitTypeAndItemID(ESlotSplitType::Skill, InSkillID))
	{
		return SkillSlot->ActiveItem();
	}
	return false;
}

bool ADWCharacter::SkillAttack(ESkillType InSkillType, int32 InAbilityIndex)
{
	if(const auto SkillSlot = Inventory->GetSlotBySplitTypeAndItemID(ESlotSplitType::Skill, GetSkillAbility(InSkillType, InAbilityIndex, true).AbilityID))
	{
		return SkillSlot->ActiveItem();
	}
	return false;
}

bool ADWCharacter::SkillAttackImpl(const FPrimaryAssetId& InSkillID)
{
	const auto AbilityData = GetSkillAbility(InSkillID);
	if(CheckWeaponType(AbilityData.WeaponType))
	{
		if(FSM->SwitchStateByClass<UDWCharacterState_Attack>())
		{
			if(AbilitySystem->TryActivateAbility(AbilityData.AbilityHandle))
			{
				AttackType = EDWCharacterAttackType::SkillAttack;
				SkillAbilityID = InSkillID;
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

bool ADWCharacter::OnPickUp(AAbilityPickUpBase* InPickUp)
{
	return Super::OnPickUp(InPickUp);
}

bool ADWCharacter::OnGenerateVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(!GenerateVoxelID.IsValid()) return false;
	
	FItemQueryInfo ItemQueryInfo = Inventory->QueryItemByRange(EItemQueryType::Remove, FAbilityItem(GenerateVoxelID, 1), -1);
	if(ItemQueryInfo.IsValid() && DoAction(EDWCharacterActionType::Generate))
	{
		if(Super::OnGenerateVoxel(InVoxelHitResult))
		{
			Inventory->RemoveItemByQueryInfo(ItemQueryInfo);
			UAchievementModuleBPLibrary::UnlockAchievement(FName("FirstGenerateVoxel"));
			return true;
		}
	}
	return false;
}

bool ADWCharacter::OnDestroyVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(DoAction(EDWCharacterActionType::Destroy))
	{
		if(Super::OnDestroyVoxel(InVoxelHitResult))
		{
			UAchievementModuleBPLibrary::UnlockAchievement(FName("FirstDestroyVoxel"));
			return true;
		}
	}
	return false;
}

bool ADWCharacter::DoAction(EDWCharacterActionType InActionType)
{
	if (!HasActionAbility(InActionType)) return false;

	const FDWCharacterActionAbilityData AbilityData = GetActionAbility(InActionType);
	const bool bSuccess = AbilitySystem->TryActivateAbility(AbilityData.AbilityHandle);
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
			if(FSM->IsCurrentStateClass<UDWCharacterState_Death>())
			{
				FSM->GetCurrentState<UDWCharacterState_Death>()->DeathEnd();
			}
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

bool ADWCharacter::CanLookAtTarget(ADWCharacter* InTargetCharacter)
{
	return !InTargetCharacter->IsDead() && FVector::Distance(GetActorLocation(), InTargetCharacter->GetActorLocation()) <= 1000.f;
}

void ADWCharacter::DoLookAtTarget(ADWCharacter* InTargetCharacter)
{
	if(!IsDodging())
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

	if(IsFloating())
	{
		if(WorldDirection.Z > -0.5f)
		{
			WorldDirection.Z = 0;
		}
		if(ScaleValue < 0.f)
		{
			ScaleValue = 0.f;
		}
	}

	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);

	if (!IsPlayer())
	{
		FHitResult hitResult;
		if (RaycastStep(hitResult))
		{
			Jump();
		}
	}
}

void ADWCharacter::SetMotionRate_Implementation(float InMovementRate, float InRotationRate)
{
	MovementRate = InMovementRate;
	RotationRate = InRotationRate;
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed() * (IsSprinting() ? 1.5f : 1) * MovementRate;
	GetCharacterMovement()->MaxSwimSpeed = GetSwimSpeed() * (IsSprinting() ? 1.5f : 1) * MovementRate;
	GetCharacterMovement()->RotationRate = FRotator(0, GetRotationSpeed() * (IsSprinting() ? 1.5f : 1) * RotationRate, 0);
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

bool ADWCharacter::IsAttacking(bool bCheckAttacked) const
{
	return !bCheckAttacked ? AbilitySystem->HasMatchingGameplayTag(GetCharacterData<UDWCharacterData>().AttackingTag) : (AttackType != EDWCharacterAttackType::None);
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

UWidgetCharacterHP* ADWCharacter::GetCharacterHPWidget() const
{
	if (CharacterHP->GetWorldWidget())
	{
		return Cast<UWidgetCharacterHP>(CharacterHP->GetWorldWidget());
	}
	return nullptr;
}

bool ADWCharacter::IsTargetable_Implementation() const
{
	return Super::IsTargetable_Implementation();
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

bool ADWCharacter::SetLevelV(int32 InLevel)
{
	if(!Super::SetLevelV(InLevel)) return false;

	if (GetCharacterHPWidget())
	{
		GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
	return true;
}

void ADWCharacter::SetControlMode_Implementation(EDWCharacterControlMode InControlMode)
{
	ControlMode = InControlMode;

	switch (ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(GetWeapon()) GetWeapon()->Execute_SetActorVisible(GetWeapon(), Execute_IsVisible(this));
			if(GetShield()) GetShield()->Execute_SetActorVisible(GetShield(), Execute_IsVisible(this));
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

void ADWCharacter::SetLockedTarget(ADWCharacter* InTargetCharacter)
{
	LockedTarget = InTargetCharacter;
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

FDWCharacterSkillAbilityData ADWCharacter::GetSkillAbility(const FPrimaryAssetId& InSkillID, bool bNeedAssembled)
{
	if(HasSkillAbility(InSkillID, bNeedAssembled))
	{
		return SkillAbilities[InSkillID];
	}
	return FDWCharacterSkillAbilityData();
}

FDWCharacterSkillAbilityData ADWCharacter::GetSkillAbility(ESkillType InSkillType, int32 InAbilityIndex, bool bNeedAssembled)
{
	if(HasSkillAbility(InSkillType, InAbilityIndex, bNeedAssembled))
	{
		TArray<FDWCharacterSkillAbilityData> Abilities = TArray<FDWCharacterSkillAbilityData>();
		for (auto& Iter : SkillAbilities)
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

bool ADWCharacter::IsAttackHitAble() const
{
	return bAttackHitAble;
}

void ADWCharacter::SetAttackHitAble(bool bValue)
{
	bAttackHitAble = bValue;
}

void ADWCharacter::ClearAttackHitTargets()
{

}

bool ADWCharacter::RaycastStep(FHitResult& OutHitResult)
{
	const FVector rayStart = GetActorLocation() + FVector::DownVector * (GetHalfHeight() - GetCharacterMovement()->MaxStepHeight);
	const FVector rayEnd = rayStart + GetMoveDirection() * (GetRadius() + AVoxelModule::Get()->GetWorldData().BlockSize * FMath::Clamp(GetMoveDirection().Size() * 0.005f, 0.5f, 1.3f));
	return UKismetSystemLibrary::LineTraceSingle(this, rayStart, rayEnd, UCommonBPLibrary::GetGameTraceType((ECollisionChannel)EDWGameTraceChannel::Step), false, {}, EDrawDebugTrace::None, OutHitResult, true);
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
		return Inventory->QueryItemBySplitType(EItemQueryType::Get, InSkillID, ESlotSplitType::Skill).IsValid();
	}
	return false;
}

bool ADWCharacter::HasSkillAbility(ESkillType InSkillType, int32 InAbilityIndex, bool bNeedAssembled) const
{
	TArray<FDWCharacterSkillAbilityData> Abilities = TArray<FDWCharacterSkillAbilityData>();
	for (auto& Iter : SkillAbilities)
	{
		if(Iter.Value.GetItemData<UAbilitySkillDataBase>().SkillType == InSkillType)
		{
			Abilities.Add(Iter.Value);
		}
	}
	if(Abilities.IsValidIndex(InAbilityIndex))
	{
		return HasSkillAbility(Abilities[InAbilityIndex].AbilityID, bNeedAssembled);
	}
	return false;
}

bool ADWCharacter::HasActionAbility(EDWCharacterActionType InActionType) const
{
	if(InActionType == EDWCharacterActionType::None) return false;
	
	return ActionAbilities.Contains(InActionType);
}

bool ADWCharacter::IsEnemy(IAbilityPawnInterface* InTarget) const
{
	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(InTarget);
	
	switch (GetNature())
	{
		case EDWCharacterNature::NPC:
		case EDWCharacterNature::AIFriendly:
		{
			return false;
		}
		default:
		{
			switch (Cast<ADWCharacter>(InTarget)->GetNature())
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
	if(IsTeamMate(TargetCharacter))
	{
		return false;
	}
	else if(TargetCharacter->GetRaceID().IsEqual(RaceID))
	{
		return false;
	}
	return true;
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

void ADWCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;

	if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetExpAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxExpAttribute())
	{
		if (GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetHealthAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxHealthAttribute())
	{
		if (GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetHealthPercent(GetHealth(), GetMaxHealth());
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
		if(GetStamina() <= 0.f)
		{
			if(!IsExhausted())
			{
				UnSprint();
				AbilitySystem->AddLooseGameplayTag(GetCharacterData<UDWCharacterData>().ExhaustedTag);
			}
		}
		else if(GetStamina() >= 10.f)
		{
			if(IsExhausted())
			{
				AbilitySystem->RemoveLooseGameplayTag(GetCharacterData<UDWCharacterData>().ExhaustedTag);
			}
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
		GetCharacterMovement()->RotationRate = FRotator(0, InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1) * RotationRate, 0);
	}
	Super::OnAttributeChange(InAttributeChangeData);
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
			if(!IsPlayer() && !GetController<ADWAIController>()->GetBlackboard()->GetTargetAgent())
			{
				GetController<ADWAIController>()->GetBlackboard()->SetTargetAgent(SourceCharacter);
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
