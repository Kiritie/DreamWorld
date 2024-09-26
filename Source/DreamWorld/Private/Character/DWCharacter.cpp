// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Character/DWCharacter.h"

#include "Achievement/AchievementModuleStatics.h"
#include "Ability/AbilityModuleStatics.h"
#include "Ability/Components/DWAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "AI/DWAIController.h"
#include "Character/DWCharacterPart.h"
#include "Common/CommonStatics.h"
#include "Scene/Actor/PhysicsVolume/PhysicsVolumeBase.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/VoxelModule.h"
#include "Widget/World/WidgetCharacterHP.h"
#include "Ability/Character/DWCharacterActionAbility.h"
#include "Ability/Character/DWCharacterAttackAbility.h"
#include "Ability/Character/DWCharacterAttributeSet.h"
#include "Ability/Character/DWCharacterSkillAbility.h"
#include "Ability/Effects/EffectBase.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Item/Equip/Shield/DWEquipShield.h"
#include "Item/Equip/Shield/DWEquipShieldData.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "Item/Equip/Weapon/DWEquipWeaponData.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/PickUp/AbilityPickUpBase.h"
#include "AI/DWAIBlackboard.h"
#include "Asset/AssetModuleStatics.h"
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
#include "Character/States/DWCharacterState_Aim.h"
#include "Common/Looking/LookingComponent.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/World/WorldWidgetComponent.h"
#include "Inventory/DWCharacterInventory.h"
#include "Scene/SceneModuleStatics.h"
#include "Setting/DWSettingModule.h"
#include "Widget/Message/WidgetMessageBox.h"

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
	static ConstructorHelpers::FClassFinder<UWidgetCharacterHP> CharacterHPClassFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/World/WBP_CharacterHP.WBP_CharacterHP_C'"));
	if(CharacterHPClassFinder.Succeeded())
	{
		CharacterHP->SetWorldWidgetClass(CharacterHPClassFinder.Class);
	}

	FSM->DefaultState = UDWCharacterState_Default::StaticClass();
	FSM->States.Empty();
	FSM->States.Add(UDWCharacterState_Aim::StaticClass());
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

	// local
	AttackAbilityIndex = 0;
	SkillAbilityItem = FAbilityItem();
	AttackType = EDWCharacterAttackType::None;
	BirthLocation = FVector(0, 0, 0);
	AIMoveLocation = EMPTY_Vector;
	AIMoveStopDistance = 0;

	CameraDoRotationTime = 0.f;
	CameraDoRotationDuration = 0.f;
	CameraDoRotationRotation = EMPTY_Rotator;

	AttackAbilities = TMap<EDWWeaponType, FDWCharacterAttackAbilityDatas>();
	SkillAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData>();
	ActionAbilities = TMap<FGameplayTag, FDWCharacterActionAbilityData>();

	AIControllerClass = ADWAIController::StaticClass();
}

void ADWCharacter::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);

	if(IsDead()) return;

	if(IsActive())
	{
		if(AIMoveLocation != EMPTY_Vector)
		{
			if(DoAIMove(AIMoveLocation, AIMoveStopDistance))
			{
				StopAIMove();
			}
		}

		if(IsSprinting())
		{
			if(IsMoving())
			{
				ModifyStamina(ATTRIBUTE_DELTAVALUE_CLAMP(this, Stamina, -GetStaminaExpendSpeed() * DeltaSeconds));
			}
			else
			{
				UnSprint();
			}
		}
		else if(IsFreeToAnim())
		{
			ModifyStamina(ATTRIBUTE_DELTAVALUE_CLAMP(this, Stamina, GetStaminaRegenSpeed() * DeltaSeconds));
		}

		if(GetStamina() < FMath::Min(GetMaxStamina() * 0.1f, 10.f))
		{
			if(!IsExhausted())
			{
				UnSprint();
				UnFly();
				UnDefend();
				AbilitySystem->AddLooseGameplayTag(GameplayTags::StateTag_Character_Exhausted);
			}
		}
		else if(GetStamina() > FMath::Min(GetMaxStamina() * 0.2f, 20.f))
		{
			if(IsExhausted())
			{
				AbilitySystem->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Exhausted);
			}
		}

		if(GetActorLocation().Z < 0)
		{
			Death();
		}
	}
}

void ADWCharacter::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);
}

void ADWCharacter::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	for(auto& Iter : Equips)
	{
		UObjectPoolModuleStatics::DespawnObject(Iter.Value);
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

			auto EquipDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilityEquipDataBase>(FName("Equip"));
			const int32 EquipNum = FMath::Clamp(FMath::Rand() < 0.3f ? FMath::RandRange(1, 2) : 0, 0, EquipDatas.Num());
			for (int32 i = 0; i < EquipNum; i++)
			{
				if(EquipDatas.IsEmpty()) break;
				const int32 tmpIndex = FMath::RandRange(0, EquipDatas.Num() - 1);
				FAbilityItem tmpItem = FAbilityItem(EquipDatas[tmpIndex]->GetPrimaryAssetId(), 1);
				SaveData.InventoryData.AddItem(tmpItem, { ESlotSplitType::Default });
				EquipDatas.RemoveAt(tmpIndex);
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

			if(FallingAttackAbility.AbilityClass)
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
				for(auto Iter : CharacterData.AttackAbilities)
				{
					Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.AbilityLevel);
					if(!AttackAbilities.Contains(Iter.WeaponType)) AttackAbilities.Add(Iter.WeaponType);
					AttackAbilities[Iter.WeaponType].Array.Add(Iter);
				}

				for(auto Iter : CharacterData.SkillAbilities)
				{
					Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.AbilityLevel);
					SkillAbilities.Add(Iter.AbilityID, Iter);
				}

				for(auto Iter : CharacterData.ActionAbilities)
				{
					Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.AbilityLevel);
					ActionAbilities.Add(Iter.AbilityClass->GetDefaultObject<UAbilityBase>()->AbilityTags.GetByIndex(0), Iter);
				}

				if(CharacterData.FallingAttackAbility.AbilityClass)
				{
					FallingAttackAbility = CharacterData.FallingAttackAbility;
					FallingAttackAbility.AbilityHandle = AbilitySystem->K2_GiveAbility(FallingAttackAbility.AbilityClass, FallingAttackAbility.AbilityLevel);
				}
			}
		}
	}

	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWCharacter::ToData()
{
	static FDWCharacterSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FCharacterSaveData>();

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

void ADWCharacter::OnFiniteStateRefresh(UFiniteStateBase* InCurrentState)
{
	switch (GetCharacterMovement()->MovementMode)
	{
		case MOVE_Flying:
		{
			FSM->SwitchStateByClass<UDWCharacterState_Fly>();
			break;
		}
		case MOVE_Swimming:
		{
			if(!InCurrentState)
			{
				FSM->SwitchStateByClass<UDWCharacterState_Walk>();
			}
			break;
		}
		default:
		{
			Super::OnFiniteStateRefresh(InCurrentState);
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
				DoAction(GameplayTags::AbilityTag_Character_Action_Use);
			}
		}
		else if(IsPlayer())
		{
			UWidgetModuleStatics::OpenUserWidget<UWidgetMessageBox>({ FString::Printf(TEXT("该%s还未准备好！"), *UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)InItem.GetType()).ToString()) });
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

bool ADWCharacter::OnPickUp(AAbilityPickUpBase* InPickUp)
{
	return Super::OnPickUp(InPickUp);
}

bool ADWCharacter::OnGenerateVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(!GenerateVoxelID.IsValid()) return false;
	
	FItemQueryInfo ItemQueryInfo = Inventory->QueryItemByRange(EItemQueryType::Remove, FAbilityItem(GenerateVoxelID, 1), -1);
	if(ItemQueryInfo.IsValid() && DoAction(GameplayTags::AbilityTag_Character_Action_Generate))
	{
		if(Super::OnGenerateVoxel(InVoxelHitResult))
		{
			Inventory->RemoveItemByQueryInfo(ItemQueryInfo);
			UAchievementModuleStatics::UnlockAchievement(FName("FirstGenerateVoxel"));
			return true;
		}
	}
	return false;
}

bool ADWCharacter::OnDestroyVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(DoAction(GameplayTags::AbilityTag_Character_Action_Destroy))
	{
		if(Super::OnDestroyVoxel(InVoxelHitResult))
		{
			UAchievementModuleStatics::UnlockAchievement(FName("FirstDestroyVoxel"));
			return true;
		}
	}
	return false;
}

void ADWCharacter::FreeToAnim()
{
	if(!IsFreeToAnim())
	{
		AbilitySystem->AddLooseGameplayTag(GameplayTags::StateTag_Character_FreeToAnim);
	}
}

void ADWCharacter::LimitToAnim()
{
	if(IsFreeToAnim())
	{
		AbilitySystem->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_FreeToAnim);
	}
}

void ADWCharacter::Interrupt(float InDuration /*= -1*/)
{
	FSM->SwitchStateByClass<UDWCharacterState_Interrupt>({ InDuration });
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
	if(!IsSprinting())
	{
		DoAction(GameplayTags::AbilityTag_Character_Action_Sprint);
	}
}

void ADWCharacter::UnSprint()
{
	if(IsSprinting())
	{
		StopAction(GameplayTags::AbilityTag_Character_Action_Sprint);
	}
}

void ADWCharacter::Crouch(bool bClientSimulation)
{
	if(!FSM->IsCurrentStateClass<UDWCharacterState_Crouch>())
	{
		FSM->SwitchStateByClass<UDWCharacterState_Crouch>({ bClientSimulation });
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
	FSM->SwitchStateByClass<UDWCharacterState_Float>({ InWaterPosZ });
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
	FSM->SwitchStateByClass<UDWCharacterState_Ride>({ InTarget });
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

void ADWCharacter::Aim()
{
	FSM->SwitchStateByClass<UDWCharacterState_Aim>();
}

void ADWCharacter::UnAim()
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Aim>())
	{
		FSM->SwitchState(nullptr);
	}
}

bool ADWCharacter::Attack(int32 InAbilityIndex /*= -1*/, const FSimpleDelegate& OnStart/* = nullptr*/, const FSimpleDelegate& OnEnd/* = nullptr*/)
{
	if(InAbilityIndex == -1) InAbilityIndex = AttackAbilityIndex;

	if(!IsFalling())
	{
		if(HasAttackAbility(InAbilityIndex))
		{
			const auto AbilityData = GetAttackAbility(InAbilityIndex);
			if(CheckWeaponType(AbilityData.WeaponType))
			{
				return FSM->SwitchStateByClass<UDWCharacterState_Attack>({ &AbilityData.AbilityHandle, (uint8)EDWCharacterAttackType::NormalAttack, InAbilityIndex, &OnStart, &OnEnd });
			}
		}
	}
	else
	{
		return FallingAttack(OnStart, OnEnd);
	}
	return false;
}

bool ADWCharacter::FallingAttack(const FSimpleDelegate& OnStart/* = nullptr*/, const FSimpleDelegate& OnEnd/* = nullptr*/)
{
	if(FallingAttackAbility.IsValid())
	{
		if(!IsFalling()) Jump();

		if(CheckWeaponType(FallingAttackAbility.WeaponType))
		{
			return FSM->SwitchStateByClass<UDWCharacterState_Attack>({ &FallingAttackAbility.AbilityHandle, (uint8)EDWCharacterAttackType::FallingAttack, &OnStart, &OnEnd });
		}
	}
	return false;
}

bool ADWCharacter::SkillAttack(const FPrimaryAssetId& InSkillID, const FSimpleDelegate& OnStart/* = nullptr*/, const FSimpleDelegate& OnEnd/* = nullptr*/)
{
	if(const auto SkillSlot = Inventory->GetSlotBySplitTypeAndItemID(ESlotSplitType::Skill, InSkillID))
	{
		return SkillSlot->ActiveItem();
	}
	return false;
}

bool ADWCharacter::SkillAttack(ESkillType InSkillType, int32 InAbilityIndex, const FSimpleDelegate& OnStart/* = nullptr*/, const FSimpleDelegate& OnEnd/* = nullptr*/)
{
	if(const auto SkillSlot = Inventory->GetSlotBySplitTypeAndItemID(ESlotSplitType::Skill, GetSkillAbility(InSkillType, InAbilityIndex, true).AbilityID))
	{
		return SkillSlot->ActiveItem();
	}
	return false;
}

bool ADWCharacter::SkillAttack(const FAbilityItem& InAbilityItem, const FSimpleDelegate& OnStart/* = nullptr*/, const FSimpleDelegate& OnEnd/* = nullptr*/)
{
	const auto AbilityData = GetSkillAbility(InAbilityItem.ID);
	if(CheckWeaponType(AbilityData.WeaponType))
	{
		return FSM->SwitchStateByClass<UDWCharacterState_Attack>({ &AbilityData.AbilityHandle, (uint8)EDWCharacterAttackType::SkillAttack, &InAbilityItem, &OnStart, &OnEnd });
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

bool ADWCharacter::DoAction(const FGameplayTag& InActionTag)
{
	if(!HasActionAbility(InActionTag)) return false;

	const FDWCharacterActionAbilityData AbilityData = GetActionAbility(InActionTag);
	const bool bSuccess = AbilitySystem->TryActivateAbility(AbilityData.AbilityHandle);
	const FGameplayAbilitySpec Spec = AbilitySystem->FindAbilitySpecForHandle(AbilityData.AbilityHandle);
	if(UDWCharacterActionAbility* Ability = Cast<UDWCharacterActionAbility>(Spec.GetPrimaryInstance()))
	{
		Ability->SetStopped(false);
	}
	return bSuccess;
}

bool ADWCharacter::StopAction(const FGameplayTag& InActionTag)
{
	if(!HasActionAbility(InActionTag)) return false;

	const FDWCharacterActionAbilityData AbilityData = GetActionAbility(InActionTag);
	const FGameplayAbilitySpec Spec = AbilitySystem->FindAbilitySpecForHandle(AbilityData.AbilityHandle);
	if(UDWCharacterActionAbility* Ability = Cast<UDWCharacterActionAbility>(Spec.GetPrimaryInstance()))
	{
		Ability->SetStopped(true);
	}
	AbilitySystem->CancelAbilityHandle(AbilityData.AbilityHandle);
	return true;
}

void ADWCharacter::EndAction(const FGameplayTag& InActionTag, bool bWasCancelled)
{
	if(!HasActionAbility(InActionTag)) return;

	if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Death))
	{
		if(FSM->IsCurrentStateClass<UDWCharacterState_Death>())
		{
			FSM->GetCurrentState<UDWCharacterState_Death>()->DeathEnd();
		}
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Crouch))
	{
		UnCrouch(false);
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Dodge))
	{
		UnDodge();
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Sprint))
	{
		UnSprint();
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Climb))
	{
		UnClimb();
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Swim))
	{
		UnSwim();
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Float))
	{
		UnFloat();
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Ride))
	{
		UnRide();
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Fly))
	{
		UnFly();
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Attack))
	{
		UnAttack();
	}
	else if(InActionTag.MatchesTag(GameplayTags::AbilityTag_Character_Action_Defend))
	{
		UnDefend();
	}
}

void ADWCharacter::AIMoveTo(FVector InTargetLocation, float InMoveStopDistance /*= 10*/, bool bMulticast /*= false*/)
{
	AIMoveLocation = InTargetLocation;
	AIMoveStopDistance = InMoveStopDistance;
}

bool ADWCharacter::DoAIMove(FVector InTargetLocation, float InMoveStopDistance /*= 10*/)
{
	if(FVector::Distance(FVector(GetActorLocation().X, GetActorLocation().Y, 0), FVector(InTargetLocation.X, InTargetLocation.Y, 0)) > InMoveStopDistance)
	{
		AddMovementInput(InTargetLocation - GetActorLocation());
		return false;
	}
	return true;
}

bool ADWCharacter::DoAIMove(ADWCharacter* InTargetCharacter, float InMoveStopDistance /*= 10*/, bool bLookAtTarget /*= false*/)
{
	if(!InTargetCharacter || !InTargetCharacter->IsValidLowLevel()) return false;

	if(GetDistance(InTargetCharacter, false, false) > InMoveStopDistance)
	{
		AddMovementInput(InTargetCharacter->GetActorLocation() - GetActorLocation());
		if(bLookAtTarget) Looking->TargetLookingOn(InTargetCharacter);
		return false;
	}
	if(bLookAtTarget) Looking->TargetLookingOff();
	return true;
}

void ADWCharacter::StopAIMove(bool bMulticast /*= false*/)
{
	AIMoveLocation = EMPTY_Vector;
}

void ADWCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if(IsFloating())
	{
		if(WorldDirection.Z > -0.5f)
		{
			WorldDirection.Z = 0;
		}
	}

	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);

	if(!IsPlayer() || UDWSettingModule::IsValid() && UDWSettingModule::Get().IsAutoJump())
	{
		FHitResult hitResult;
		if(RaycastStep(hitResult))
		{
			Jump();
		}
	}
}

void ADWCharacter::SetMotionRate_Implementation(float InMovementRate, float InRotationRate)
{
	MovementRate = InMovementRate;
	RotationRate = InRotationRate;
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed() * (IsSprinting() ? 1.5f : 1.f) * MovementRate;
	GetCharacterMovement()->MaxSwimSpeed = GetSwimSpeed() * (IsSprinting() ? 1.5f : 1.f) * MovementRate;
	GetCharacterMovement()->MaxFlySpeed = GetFlySpeed() * (IsSprinting() ? 1.5f : 1.f) * MovementRate;
	GetCharacterMovement()->RotationRate = FRotator(0, GetRotationSpeed() * (IsSprinting() ? 1.5f : 1.f) * RotationRate, 0.f);
}

bool ADWCharacter::IsExhausted() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Exhausted);
}

bool ADWCharacter::IsFreeToAnim() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_FreeToAnim);
}

bool ADWCharacter::IsDodging() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Dodging);
}

bool ADWCharacter::IsSprinting() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Sprinting);
}

bool ADWCharacter::IsCrouching(bool bReally) const
{
	return !bReally ? AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Crouching) : GetCharacterMovement()->IsCrouching();
}

bool ADWCharacter::IsSwimming(bool bReally) const
{
	return !bReally ? AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Swimming) : GetCharacterMovement()->IsSwimming();
}

bool ADWCharacter::IsFloating() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Floating);
}

bool ADWCharacter::IsAiming() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Aiming);
}

bool ADWCharacter::IsAttacking(EDWCharacterAttackType InAttackType) const
{
	return InAttackType == EDWCharacterAttackType::None ? AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Attacking) : AttackType == InAttackType;
}

bool ADWCharacter::IsDefending() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Defending);
}

bool ADWCharacter::IsClimbing() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Climbing);
}

bool ADWCharacter::IsRiding() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Riding);
}

bool ADWCharacter::IsFlying(bool bReally) const
{
	return !bReally ? AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Flying) : GetCharacterMovement()->IsFlying();
}

bool ADWCharacter::IsInterrupting() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::StateTag_Character_Interrupting);
}

UWidgetCharacterHP* ADWCharacter::GetCharacterHPWidget() const
{
	if(CharacterHP->GetWorldWidget())
	{
		return Cast<UWidgetCharacterHP>(CharacterHP->GetWorldWidget());
	}
	return nullptr;
}

void ADWCharacter::SetNameV(FName InName)
{
	Super::SetNameV(InName);

	if(GetCharacterHPWidget())
	{
		GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWCharacter::SetRaceID(FName InRaceID)
{
	Super::SetRaceID(InRaceID);
	
	if(GetCharacterHPWidget())
	{
		GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

bool ADWCharacter::SetLevelA(int32 InLevel)
{
	if(!Super::SetLevelA(InLevel)) return false;

	if(GetCharacterHPWidget())
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

	if(GetCharacterHPWidget())
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
	if(Equips.Contains(InPartType))
	{
		return Equips[InPartType];
	}
	return nullptr;
}

ADWEquipWeapon* ADWCharacter::GetWeapon() const
{
	return IsAiming() ? GetEquip<ADWEquipWeapon>(EDWEquipPartType::LeftHand) : GetEquip<ADWEquipWeapon>(EDWEquipPartType::RightHand);
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
	if(InAbilityIndex == -1) InAbilityIndex = FMath::RandRange(0, SkillAbilities.Num() -1);
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
		return Abilities[InAbilityIndex];
	}
	return FDWCharacterSkillAbilityData();
}

FDWCharacterActionAbilityData ADWCharacter::GetActionAbility(const FGameplayTag& InActionTag)
{
	if(HasActionAbility(InActionTag))
	{
		return ActionAbilities[InActionTag];
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
	const FVector rayEnd = rayStart + GetMoveDirection(true) * (GetRadius() + 100.f * FMath::Lerp(0.35f, 1.25f, GetMoveVelocity(true).Size() / 500.f));
	return UKismetSystemLibrary::LineTraceSingle(this, rayStart, rayEnd, USceneModuleStatics::GetTraceMapping(FName("Step")).GetTraceType(), false, {}, EDrawDebugTrace::None, OutHitResult, true);
}

bool ADWCharacter::IsTargetAble_Implementation(APawn* InPlayerPawn) const
{
	return Super::IsTargetAble_Implementation(InPlayerPawn) && GetOwnerRider() != InPlayerPawn;
}

bool ADWCharacter::IsLookAtAble_Implementation(AActor* InLookerActor) const
{
	return Super::IsLookAtAble_Implementation(InLookerActor) && GetOwnerRider() != InLookerActor;
}

bool ADWCharacter::CanLookAtTarget()
{
	return Super::CanLookAtTarget() && !IsDodging();
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

bool ADWCharacter::HasActionAbility(const FGameplayTag& InActionTag) const
{
	return ActionAbilities.Contains(InActionTag);
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
	return Super::IsEnemy(InTarget);
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
	const UDWCharacterData& CharacterData = GetCharacterData<UDWCharacterData>();
	switch(CharacterData.Nature)
	{
		case EDWCharacterNature::AINeutral:
		{
			if(UDWAIBlackboard* Blackboard = GetBlackboard<UDWAIBlackboard>())
			{
				return !Blackboard->GetIsExcessived() ? CharacterData.DefaultBehaviorTree : CharacterData.ExcessiveBehaviorTree;
			}
		}
		case EDWCharacterNature::NPC:
		case EDWCharacterNature::AIFriendly:
		case EDWCharacterNature::AIHostile:
		{
			return CharacterData.DefaultBehaviorTree;
		}
		default: break;
	}
	return nullptr;
}

void ADWCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;

	if(InAttributeChangeData.Attribute == GetExpAttribute() || InAttributeChangeData.Attribute == GetMaxExpAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetHealthAttribute() || InAttributeChangeData.Attribute == GetMaxHealthAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetHealthPercent(GetHealth(), GetMaxHealth());
		}
	}
	else if(InAttributeChangeData.Attribute == GetManaAttribute() || InAttributeChangeData.Attribute == GetMaxManaAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetManaPercent(GetMana(), GetMaxMana());
		}
	}
	else if(InAttributeChangeData.Attribute == GetStaminaAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
	}
	else if(InAttributeChangeData.Attribute == GetMaxStaminaAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
	}
	else if(InAttributeChangeData.Attribute == GetMoveSpeedAttribute())
	{
		GetCharacterMovement()->MaxWalkSpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1.f) * MovementRate;
	}
	else if(InAttributeChangeData.Attribute == GetSwimSpeedAttribute())
	{
		GetCharacterMovement()->MaxSwimSpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1.f) * MovementRate;
	}
	else if(InAttributeChangeData.Attribute == GetFlySpeedAttribute())
	{
		GetCharacterMovement()->MaxFlySpeed = InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1.f) * MovementRate;
	}
	else if(InAttributeChangeData.Attribute == GetRotationSpeedAttribute())
	{
		GetCharacterMovement()->RotationRate = FRotator(0, InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1.f) * RotationRate, 0);
	}
	Super::OnAttributeChange(InAttributeChangeData);
}

void ADWCharacter::HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, bool bHasDefend, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleDamage(DamageType, LocalDamageDone, bHasCrited, bHasDefend, HitResult, SourceTags, SourceActor);

	if(!IsDead())
	{
		if(!bHasDefend)
		{
			DoAction(GameplayTags::AbilityTag_Character_Action_GetHit);
		}

		ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor);

		if(SourceCharacter && SourceCharacter != this)
		{
			if(UDWAIBlackboard* Blackboard = GetBlackboard<UDWAIBlackboard>())
			{
				Blackboard->SetTargetAgent(SourceCharacter);
				Blackboard->SetIsExcessived(true);
			}
		}
	}
}

void ADWCharacter::HandleRecovery(const float LocalRecoveryDone, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleRecovery(LocalRecoveryDone, HitResult, SourceTags, SourceActor);
}

void ADWCharacter::HandleInterrupt(const float InterruptDuration, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleInterrupt(InterruptDuration, HitResult, SourceTags, SourceActor);

	if(InterruptDuration > 0.f)
	{
		Interrupt(InterruptDuration * (1.f - GetToughnessRate()));
	}
}
