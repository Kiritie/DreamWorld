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
#include "Voxel/DWVoxelChunk.h"
#include "Widget/World/WidgetCharacterHP.h"
#include "Ability/Attributes/DWCharacterAttributeSet.h"
#include "Ability/Effects/EffectBase.h"
#include "Item/Equip/Shield/DWEquipShield.h"
#include "Item/Equip/Shield/DWEquipShieldData.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "Item/Equip/Weapon/DWEquipWeaponData.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/PickUp/AbilityPickUpBase.h"
#include "AI/DWAIBlackboard.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Character/States/DWCharacterState_Death.h"
#include "Character/States/DWCharacterState_Spawn.h"
#include "Character/States/DWCharacterState_Defend.h"
#include "Character/States/DWCharacterState_Dodge.h"
#include "Character/States/DWCharacterState_Float.h"
#include "Character/States/DWCharacterState_Ride.h"
#include "FSM/Components/FSMComponent.h"
#include "Ability/Inventory/Slot/AbilityInventorySkillSlotBase.h"
#include "Character/States/DWCharacterState_Aim.h"
#include "Common/Looking/LookingComponent.h"
#include "Event/EventModuleStatics.h"
#include "Event/Handle/Voxel/EventHandle_VoxelWorldModeChanged.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/World/WorldWidgetComponent.h"
#include "Inventory/DWCharacterInventory.h"
#include "Scene/SceneModuleStatics.h"
#include "Setting/DWSettingModule.h"
#include "Widget/Message/WidgetMessageBox.h"
#include "Ability/Abilities/CharacterAbilityBase.h"
#include "Ability/Character/States/AbilityCharacterState_Crouch.h"
#include "Ability/Character/States/AbilityCharacterState_Fall.h"
#include "Ability/Character/States/AbilityCharacterState_Interrupt.h"
#include "Ability/Character/States/AbilityCharacterState_Jump.h"
#include "Ability/Character/States/AbilityCharacterState_Static.h"
#include "Ability/Character/States/AbilityCharacterState_Swim.h"
#include "Ability/Character/States/AbilityCharacterState_Walk.h"
#include "Character/States/DWCharacterState_Climb.h"
#include "Character/States/DWCharacterState_Fly.h"

// Sets default values
ADWCharacter::ADWCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UDWAbilitySystemComponent>(FName("AbilitySystem")).
		SetDefaultSubobjectClass<UDWCharacterAttributeSet>(FName("AttributeSet")).
		SetDefaultSubobjectClass<UDWCharacterInventory>(FName("Inventory")))
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

	FSM->DefaultState = UDWCharacterState_Spawn::StaticClass();
	FSM->FinalState = UDWCharacterState_Death::StaticClass();
	
	FSM->States.Empty();
	FSM->States.Add(UDWCharacterState_Aim::StaticClass());
	FSM->States.Add(UDWCharacterState_Attack::StaticClass());
	FSM->States.Add(UDWCharacterState_Climb::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Crouch::StaticClass());
	FSM->States.Add(UDWCharacterState_Death::StaticClass());
	FSM->States.Add(UDWCharacterState_Spawn::StaticClass());
	FSM->States.Add(UDWCharacterState_Defend::StaticClass());
	FSM->States.Add(UDWCharacterState_Dodge::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Fall::StaticClass());
	FSM->States.Add(UDWCharacterState_Float::StaticClass());
	FSM->States.Add(UDWCharacterState_Fly::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Interrupt::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Jump::StaticClass());
	FSM->States.Add(UDWCharacterState_Ride::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Static::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Swim::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Walk::StaticClass());

	// stats
	ControlMode = EDWCharacterControlMode::Fighting;
	TeamID = NAME_None;
	
	Equips = TMap<EDWEquipPartType, AAbilityEquipBase*>();

	RidingTarget = nullptr;

	// local
	SkillAbilityItem = FAbilityItem();
	AttackType = EDWCharacterAttackType::None;
	AIMoveLocation = EMPTY_Vector;
	AIMoveStopDistance = 0;

	CameraDoRotationTime = 0.f;
	CameraDoRotationDuration = 0.f;
	CameraDoRotationRotation = EMPTY_Rotator;

	AttackAbilityQueues = TMap<EDWWeaponType, FDWCharacterAttackAbilityQueue>();
	SkillAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData>();

	AIControllerClass = ADWAIController::StaticClass();
}

void ADWCharacter::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
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

		ModifyMana(ATTRIBUTE_DELTAVALUE_CLAMP(this, Mana, GetManaRegenSpeed() * DeltaSeconds));

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
				UnClimb();
				UnFly();
				UnDefend();
				AbilitySystem->AddLooseGameplayTag(GameplayTags::State_Character_Exhausted);
			}
		}
		else if(GetStamina() > FMath::Min(GetMaxStamina() * 0.2f, 20.f))
		{
			if(IsExhausted())
			{
				AbilitySystem->RemoveLooseGameplayTag(GameplayTags::State_Character_Exhausted);
			}
		}

		if(GetActorLocation().Z < 0)
		{
			Kill(this);
		}
	}
}

void ADWCharacter::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);

	UEventModuleStatics::SubscribeEvent<UEventHandle_VoxelWorldModeChanged>(this, GET_FUNCTION_NAME_THISCLASS(OnWorldModeChanged));
}

void ADWCharacter::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelWorldModeChanged>(this, GET_FUNCTION_NAME_THISCLASS(OnWorldModeChanged));

	for(auto& Iter : Equips)
	{
		UObjectPoolModuleStatics::DespawnObject(Iter.Value);
	}
	Equips.Empty();
}

void ADWCharacter::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWCharacterSaveData>();

	if(PHASEC(InPhase, EPhase::Primary))
	{
		SetControlMode(SaveData.ControlMode);
		SetTeamID(SaveData.TeamID);

		if(!SaveData.IsSaved())
		{
			const UDWCharacterData& CharacterData = GetCharacterData<UDWCharacterData>();
			
			for(auto Iter : CharacterData.AttackAbilities)
			{
				Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.Level);
				if(!AttackAbilityQueues.Contains(Iter.WeaponType)) AttackAbilityQueues.Add(Iter.WeaponType);
				AttackAbilityQueues[Iter.WeaponType].AbilityDatas.Add(Iter);
			}

			for(auto Iter : CharacterData.SkillAbilities)
			{
				Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.Level);
				SkillAbilities.Add(Iter.ID, Iter);
			}

			if(CharacterData.FallingAttackAbility.AbilityClass)
			{
				FallingAttackAbility = CharacterData.FallingAttackAbility;
				FallingAttackAbility.AbilityHandle = AbilitySystem->K2_GiveAbility(FallingAttackAbility.AbilityClass, FallingAttackAbility.Level);
			}
		}
		else
		{
			AttackAbilityQueues = SaveData.AttackAbilityQueues;
			for(auto& Iter : AttackAbilityQueues)
			{
				for(auto& Iter1 : Iter.Value.AbilityDatas)
				{
					Iter1.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter1.AbilityClass, Iter1.Level);
				}
			}
		
			SkillAbilities = SaveData.SkillAbilities;
			for(auto& Iter : SkillAbilities)
			{
				Iter.Value.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.Value.AbilityClass, Iter.Value.Level);
			}

			FallingAttackAbility = SaveData.FallingAttackAbility;
			if(FallingAttackAbility.AbilityClass)
			{
				FallingAttackAbility.AbilityHandle = AbilitySystem->K2_GiveAbility(FallingAttackAbility.AbilityClass, FallingAttackAbility.Level);
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
	SaveData.ControlMode = ControlMode;

	SaveData.FallingAttackAbility = FallingAttackAbility;
	SaveData.AttackAbilityQueues = AttackAbilityQueues;
	SaveData.SkillAbilities = SkillAbilities;

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
				FSM->SwitchStateByClass<UAbilityCharacterState_Walk>();
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

void ADWCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassive);

	if(bPassive)
	{
		switch (InInteractAction)
		{
			case EInteractAction::Revive:
			{
				Revive(Cast<IAbilityVitalityInterface>(InInteractionAgent));
			}
			default: break;
		}
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
				DoAction(GameplayTags::Ability_Character_Action_Use);
			}
		}
		else if(IsPlayer())
		{
			UWidgetModuleStatics::OpenUserWidget<UWidgetMessageBox>({ FText::FromString(FString::Printf(TEXT("该%s还未准备好！"), *UCommonStatics::GetEnumValueDisplayName(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)InItem.GetType()).ToString())) });
		}
	}
}

void ADWCharacter::OnRemoveItem(const FAbilityItem& InItem)
{
	Super::OnRemoveItem(InItem);
}

void ADWCharacter::OnDeactiveItem(const FAbilityItem& InItem, bool bPassive)
{
	Super::OnDeactiveItem(InItem, bPassive);
}

void ADWCharacter::OnDiscardItem(const FAbilityItem& InItem, bool bInPlace)
{
	Super::OnDiscardItem(InItem, bInPlace);
}

void ADWCharacter::OnSelectItem(ESlotSplitType InSplitType, const FAbilityItem& InItem)
{
	Super::OnSelectItem(InSplitType, InItem);
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
	
	FItemQueryData ItemQueryData = Inventory->QueryItemByRange(EItemQueryType::Remove, FAbilityItem(GenerateVoxelID, 1), -1);
	if(ItemQueryData.IsValid() && DoAction(GameplayTags::Ability_Character_Action_Generate))
	{
		if(Super::OnGenerateVoxel(InVoxelHitResult))
		{
			Inventory->RemoveItemByQueryData(ItemQueryData);
			UAchievementModuleStatics::UnlockAchievement(FName("FirstGenerateVoxel"));
			return true;
		}
	}
	return false;
}

bool ADWCharacter::OnDestroyVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(DoAction(GameplayTags::Ability_Character_Action_Destroy))
	{
		if(Super::OnDestroyVoxel(InVoxelHitResult))
		{
			UAchievementModuleStatics::UnlockAchievement(FName("FirstDestroyVoxel"));
			return true;
		}
	}
	return false;
}

void ADWCharacter::OnWorldModeChanged(UObject* InSender, UEventHandle_VoxelWorldModeChanged* InEventHandle)
{
	if(FSM->IsCurrentStateClass<UDWCharacterState_Spawn>()) return;
	
	switch(InEventHandle->WorldMode)
	{
		case EVoxelWorldMode::Preview:
		{
			Static();
			break;
		}
		case EVoxelWorldMode::Default:
		{
			UnStatic();
			break;
		}
		default: break;
	}
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
		DoAction(GameplayTags::Ability_Character_Action_Sprint);
	}
}

void ADWCharacter::UnSprint()
{
	if(IsSprinting())
	{
		StopAction(GameplayTags::Ability_Character_Action_Sprint);
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

bool ADWCharacter::Attack(int32 InAbilityIndex /*= -1*/, const FSimpleDelegate& OnCompleted/* = nullptr*/)
{
	if(InAbilityIndex == -1) InAbilityIndex = GetAttackAbilityQueue().AbilityIndex;

	if(!IsFalling())
	{
		const auto AbilityData = GetAttackAbility(InAbilityIndex);
		if(AbilityData.IsValid() && CheckWeaponType(AbilityData.WeaponType))
		{
			return FSM->SwitchStateByClass<UDWCharacterState_Attack>({ &AbilityData.AbilityHandle, (uint8)EDWCharacterAttackType::NormalAttack, InAbilityIndex, &OnCompleted });
		}
	}
	else
	{
		return FallingAttack(OnCompleted);
	}
	return false;
}

bool ADWCharacter::FallingAttack(const FSimpleDelegate& OnCompleted/* = nullptr*/)
{
	if(FallingAttackAbility.IsValid() && CheckWeaponType(FallingAttackAbility.WeaponType))
	{
		if(!IsFalling()) Jump();

		return FSM->SwitchStateByClass<UDWCharacterState_Attack>({ &FallingAttackAbility.AbilityHandle, (uint8)EDWCharacterAttackType::FallingAttack, &OnCompleted });
	}
	return false;
}

bool ADWCharacter::SkillAttack(const FPrimaryAssetId& InSkillID, const FSimpleDelegate& OnCompleted/* = nullptr*/)
{
	if(const auto SkillSlot = Inventory->GetSlotBySplitTypeAndItemID(ESlotSplitType::Skill, InSkillID))
	{
		return SkillSlot->ActiveItem();
	}
	return false;
}

bool ADWCharacter::SkillAttack(ESkillType InSkillType, int32 InAbilityIndex, const FSimpleDelegate& OnCompleted/* = nullptr*/)
{
	if(const auto SkillSlot = Inventory->GetSlotBySplitTypeAndItemID(ESlotSplitType::Skill, GetSkillAbility(InSkillType, InAbilityIndex, true).ID))
	{
		return SkillSlot->ActiveItem();
	}
	return false;
}

bool ADWCharacter::SkillAttack(const FAbilityItem& InAbilityItem, const FSimpleDelegate& OnCompleted/* = nullptr*/)
{
	const auto AbilityData = GetSkillAbility(InAbilityItem.ID);
	if(AbilityData.IsValid())
	{
		return FSM->SwitchStateByClass<UDWCharacterState_Attack>({ &AbilityData.AbilityHandle, (uint8)EDWCharacterAttackType::SkillAttack, &InAbilityItem, &OnCompleted });
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
	return Super::DoAction(InActionTag);
}

bool ADWCharacter::StopAction(const FGameplayTag& InActionTag)
{
	return Super::StopAction(InActionTag);
}

void ADWCharacter::EndAction(const FGameplayTag& InActionTag, bool bWasCancelled)
{
	Super::EndAction(InActionTag, bWasCancelled);

	if(InActionTag.MatchesTag(GameplayTags::Ability_Character_Action_Dodge))
	{
		UnDodge();
	}
	else if(InActionTag.MatchesTag(GameplayTags::Ability_Character_Action_Sprint))
	{
		UnSprint();
	}
	else if(InActionTag.MatchesTag(GameplayTags::Ability_Character_Action_Ride))
	{
		UnRide();
	}
	else if(InActionTag.MatchesTag(GameplayTags::Ability_Character_Action_Attack))
	{
		UnAttack();
	}
	else if(InActionTag.MatchesTag(GameplayTags::Ability_Character_Action_Defend))
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

bool ADWCharacter::DoAIMove(AActor* InTargetActor, float InMoveStopDistance /*= 10*/, bool bLookAtTarget /*= false*/)
{
	if(!InTargetActor || !InTargetActor->IsValidLowLevel()) return false;

	if(GetDistance(InTargetActor, false, false) > InMoveStopDistance)
	{
		AddMovementInput(InTargetActor->GetActorLocation() - GetActorLocation());
		if(bLookAtTarget) Looking->TargetLookingOn(InTargetActor);
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
			WorldDirection.Z = 0.f;
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

void ADWCharacter::SetMotionRate(float InMovementRate, float InRotationRate)
{
	Super::SetMotionRate(InMovementRate, InRotationRate);

	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed() * (IsSprinting() ? 1.5f : 1.f) * MovementRate;
	GetCharacterMovement()->MaxSwimSpeed = GetSwimSpeed() * (IsSprinting() ? 1.5f : 1.f) * MovementRate;
	GetCharacterMovement()->MaxFlySpeed = GetFlySpeed() * (IsSprinting() ? 1.5f : 1.f) * MovementRate;
	GetCharacterMovement()->RotationRate = FRotator(0, GetRotationSpeed() * (IsSprinting() ? 1.5f : 1.f) * RotationRate, 0.f);
}

UWidgetCharacterHP* ADWCharacter::GetCharacterHPWidget() const
{
	if(CharacterHP->GetWorldWidget())
	{
		return Cast<UWidgetCharacterHP>(CharacterHP->GetWorldWidget());
	}
	return nullptr;
}

void ADWCharacter::SetNameA(FName InName)
{
	Super::SetNameA(InName);

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
			if(ADWEquipWeapon* Weapon = GetWeapon())
			{
				Execute_SetActorVisible(Weapon, Execute_IsVisible(this));
			}
			if(ADWEquipShield* Shield = GetShield())
			{
				Execute_SetActorVisible(Shield, Execute_IsVisible(this));
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			if(ADWEquipWeapon* Weapon = GetWeapon())
			{
				Execute_SetActorVisible(Weapon, false);
			}
			if(ADWEquipShield* Shield = GetShield())
			{
				Execute_SetActorVisible(Shield, false);
			}
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
	for(auto& Iter : Equips)
	{
		if(ADWEquipWeapon* Weapon = Cast<ADWEquipWeapon>(Iter.Value))
		{
			return Weapon;
		}
	}
	return nullptr;
}

EDWWeaponType ADWCharacter::GetWeaponType() const
{
	if(ADWEquipWeapon* Weapon = GetWeapon())
	{
		return Weapon->GetItemData<UDWEquipWeaponData>().WeaponType;
	}
	return EDWWeaponType::None;
}

bool ADWCharacter::CheckWeaponType(EDWWeaponType InWeaponType) const
{
	return InWeaponType == EDWWeaponType::None || GetWeaponType() == InWeaponType;
}

TSubclassOf<AAbilityProjectileBase> ADWCharacter::GetWeaponProjectileClass() const
{
	if(ADWEquipWeapon* Weapon = GetWeapon())
	{
		return Weapon->GetItemData<UDWEquipWeaponData>().ProjectileClass;
	}
	return nullptr;
}

ADWEquipShield* ADWCharacter::GetShield() const
{
	for(auto& Iter : Equips)
	{
		if(ADWEquipShield* Shield = Cast<ADWEquipShield>(Iter.Value))
		{
			return Shield;
		}
	}
	return nullptr;
}

EDWShieldType ADWCharacter::GetShieldType() const
{
	if(ADWEquipShield* Shield = GetShield())
	{
		return Shield->GetItemData<UDWEquipShieldData>().ShieldType;
	}
	return EDWShieldType::None;
}

bool ADWCharacter::CheckShieldType(EDWShieldType InShieldType) const
{
	return InShieldType == EDWShieldType::None || GetShieldType() == InShieldType;
}

FDWCharacterAttackAbilityData ADWCharacter::GetAttackAbility(int32 InAbilityIndex)
{
	if(InAbilityIndex == -1) InAbilityIndex = GetAttackAbilityQueue().AbilityIndex;
	if(HasAttackAbility(InAbilityIndex))
	{
		return GetAttackAbilities()[InAbilityIndex];
	}
	return FDWCharacterAttackAbilityData();
}

bool ADWCharacter::HasAttackAbility(int32 InAbilityIndex) const
{
	if(AttackAbilityQueues.Contains(GetWeaponType()))
	{
		return AttackAbilityQueues[GetWeaponType()].AbilityDatas.IsValidIndex(InAbilityIndex);
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
		if(Iter.Value.GetData<UAbilitySkillDataBase>().SkillType == InSkillType)
		{
			Abilities.Add(Iter.Value);
		}
	}
	if(Abilities.IsValidIndex(InAbilityIndex))
	{
		return HasSkillAbility(Abilities[InAbilityIndex].ID, bNeedAssembled);
	}
	return false;
}

FDWCharacterAttackAbilityQueue& ADWCharacter::GetAttackAbilityQueue()
{
	if(AttackAbilityQueues.Contains(GetWeaponType()))
	{
		return AttackAbilityQueues[GetWeaponType()];
	}
	static FDWCharacterAttackAbilityQueue Empty;
	return Empty;
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
			if(Iter.Value.GetData<UAbilitySkillDataBase>().SkillType == InSkillType)
			{
				Abilities.Add(Iter.Value);
			}
		}
		return Abilities[InAbilityIndex];
	}
	return FDWCharacterSkillAbilityData();
}

TArray<FDWCharacterAttackAbilityData> ADWCharacter::GetAttackAbilities() const
{
	if(AttackAbilityQueues.Contains(GetWeaponType()))
	{
		return AttackAbilityQueues[GetWeaponType()].AbilityDatas;
	}
	return TArray<FDWCharacterAttackAbilityData>();
}

bool ADWCharacter::IsHitAble() const
{
	return bHitAble;
}

void ADWCharacter::SetHitAble(bool bValue)
{
	bHitAble = bValue;
}

void ADWCharacter::ClearHitTargets()
{

}

TArray<AActor*> ADWCharacter::GetHitTargets() const
{
	return TArray<AActor*>();
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

void ADWCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
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

void ADWCharacter::HandleDamage(EDamageType DamageType, float DamageValue, bool bHasCrited, bool bHasDefend, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleDamage(DamageType, DamageValue, bHasCrited, bHasDefend, HitResult, SourceTags, SourceActor);

	if(!IsDead())
	{
		if(!bHasDefend)
		{
			DoAction(GameplayTags::Ability_Character_Action_GetHit);
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

void ADWCharacter::HandleRecovery(float RecoveryValue, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleRecovery(RecoveryValue, HitResult, SourceTags, SourceActor);
}

void ADWCharacter::HandleInterrupt(float InterruptDuration, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleInterrupt(InterruptDuration, HitResult, SourceTags, SourceActor);

	if(InterruptDuration > 0.f)
	{
		Interrupt(InterruptDuration * (1.f - GetToughnessRate()));
	}
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

bool ADWCharacter::IsExhausted() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::State_Character_Exhausted);
}

bool ADWCharacter::IsDodging() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::State_Character_Dodging);
}

bool ADWCharacter::IsSprinting() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::State_Character_Sprinting);
}

bool ADWCharacter::IsAiming() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::State_Character_Aiming);
}

bool ADWCharacter::IsAttacking(EDWCharacterAttackType InAttackType) const
{
	return InAttackType == EDWCharacterAttackType::None ? AbilitySystem->HasMatchingGameplayTag(GameplayTags::State_Character_Attacking) : AttackType == InAttackType;
}

bool ADWCharacter::IsDefending() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::State_Character_Defending);
}

bool ADWCharacter::IsRiding() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::State_Character_Riding);
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
