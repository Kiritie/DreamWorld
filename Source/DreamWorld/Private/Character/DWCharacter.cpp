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
#include "Ability/Abilities/CharacterAbilityBase.h"
#include "Ability/Character/States/AbilityCharacterState_Crouch.h"
#include "Ability/Character/States/AbilityCharacterState_Fall.h"
#include "Ability/Character/States/AbilityCharacterState_Interrupt.h"
#include "Ability/Character/States/AbilityCharacterState_Jump.h"
#include "Ability/Character/States/AbilityCharacterState_Static.h"
#include "Ability/Character/States/AbilityCharacterState_Walk.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/States/DWCharacterState_Climb.h"
#include "Character/States/DWCharacterState_Fly.h"
#include "Character/States/DWCharacterState_Sleep.h"
#include "Character/States/DWCharacterState_Swim.h"
#include "Common/DWCommonStatics.h"
#include "Inventory/Slot/DWInventoryEquipSlot.h"
#include "Item/Skill/DWSkillData.h"
#include "Item/Talent/DWTalentData.h"
#include "Voxel/Voxels/Data/VoxelData.h"
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"
#include "Widget/Common/WidgetProgressBox.h"
#include "Widget/Head/WidgetHeadBox.h"
#include "Widget/Head/WidgetTeamMateHeadBox.h"

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
	CharacterHP->SetAutoCreate(false);
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
	FSM->States.Add(UDWCharacterState_Sleep::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Static::StaticClass());
	FSM->States.Add(UDWCharacterState_Swim::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Walk::StaticClass());

	// stats
	ControlMode = EDWCharacterControlMode::Fighting;
	WeaponGroup = EDWWeaponGroup::Group1;
	TeamID = NAME_None;

	TalentPoint = 0;
	TalentItems = TArray<FAbilityItems>();
	
	Equips = TMap<EDWEquipPart, ADWEquip*>();

	// local
	SkillAttackAbilityItem = FAbilityItem();
	AttackType = EDWCharacterAttackType::None;
	AttackWeaponPart = EDWWeaponPart::None;
	
	AIMoveLocation = EMPTY_Vector;
	AIMoveStopDistance = 0;

	CameraDoRotationTime = 0.f;
	CameraDoRotationDuration = 0.f;
	CameraDoRotationRotation = EMPTY_Rotator;

	StatDamageRemainTime = 0.f;

	AttackAbilityQueues = TMap<EDWWeaponType, FDWCharacterAttackAbilityQueue>();
	FallingAttackAbilities = TMap<EDWWeaponType, FDWCharacterFallingAttackAbilityData>();
	SkillAttackAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAttackAbilityData>();

	AIControllerClass = ADWAIController::StaticClass();
}

void ADWCharacter::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);

	UEventModuleStatics::SubscribeEvent<UEventHandle_VoxelWorldModeChanged>(this, GET_FUNCTION_NAME_THISCLASS(OnWorldModeChanged));

	CharacterHP->CreateWorldWidget();
}

void ADWCharacter::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelWorldModeChanged>(this, GET_FUNCTION_NAME_THISCLASS(OnWorldModeChanged));

	CharacterHP->DestroyWorldWidget();

	for(auto& Iter : Equips)
	{
		UObjectPoolModuleStatics::DespawnObject(Iter.Value);
	}
	Equips.Empty();
}

void ADWCharacter::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWCharacter::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);

	if(IsActive())
	{
		if(AIMoveLocation != EMPTY_Vector)
		{
			if(DoAIMove(AIMoveLocation, AIMoveStopDistance))
			{
				StopAIMove();
			}
		}

		float FinalStatDamage = 0.f;

		if(IsPlayer())
		{
			if(GetHunger() > 0.f)
			{
				ModifyHunger(-GetHungerExpendSpeed() * DeltaSeconds * (IsSprinting() ? 2.f : 1.f));
			}
		
			if(GetThirst() > 0.f)
			{
				ModifyThirst(-GetThirstExpendSpeed() * DeltaSeconds);
			}

			if(GetThirst() <= 0.f)
			{
				FinalStatDamage += 10.f;
			}
		}

		if(!IsSwimming() && GetOxygen() < 100.f)
		{
			ModifyOxygen(GetOxygenRegenSpeed() * DeltaSeconds);
		}
		
		if(GetOxygen() <= 0.f)
		{
			FinalStatDamage += 20.f;
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
			ModifyStamina(ATTRIBUTE_DELTAVALUE_CLAMP(this, Stamina, GetStaminaRegenSpeed() * ((GetHunger() / 100.f) * 0.9f + 0.1f) * DeltaSeconds));
		}

		if(GetStamina() < FMath::Min(GetMaxStamina() * 0.1f, 10.f))
		{
			if(!IsExhausted())
			{
				UnSprint();
				UnClimb();
				UnFly();
				UnAim();
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
		else if(FinalStatDamage > 0.f)
		{
			if(StatDamageRemainTime <= 0.f)
			{
				StatDamageRemainTime = 1.f;
				
				UEffectBase* Effect = UObjectPoolModuleStatics::SpawnObject<UEffectBase>();

				FGameplayModifierInfo ModifierInfo;
				ModifierInfo.Attribute = GET_GAMEPLAYATTRIBUTE(UVitalityAttributeSetBase, RealDamage);
				ModifierInfo.ModifierOp = EGameplayModOp::Override;
				ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FinalStatDamage);

				Effect->Modifiers.Add(ModifierInfo);
			
				FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(Effect, 0, EffectContext);

				UObjectPoolModuleStatics::DespawnObject(Effect);
			}
			StatDamageRemainTime -= DeltaSeconds;
		}
	}
}

void ADWCharacter::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	auto& SaveData = InSaveData->CastRef<FDWCharacterSaveData>();

	if(PHASEC(InPhase, EPhase::Primary))
	{
		SetControlMode(SaveData.ControlMode);
		SetWeaponGroup(SaveData.WeaponGroup);
		SetTeamID(SaveData.TeamID);

		SetTalentPoint(SaveData.TalentPoint);
		SetTalentItems(SaveData.TalentItems);

		for(auto& Iter1 : TalentItems)
		{
			for(auto& Iter2 : Iter1.Items)
			{
				Iter2.Handle = AbilitySystem->K2_GiveAbility(Iter2.GetData().AbilityClass, Iter2.Level);
				if(Iter2.Level > 0)
				{
					AbilitySystem->TryActivateAbility(Iter2.Handle);
				}
			}			
		}

		if(!SaveData.IsSaved())
		{
			const UDWCharacterData& CharacterData = GetCharacterData<UDWCharacterData>();
			
			for(auto Iter1 : CharacterData.AttackAbilities)
			{
				for(auto Iter2 : Iter1.Value.Abilities)
				{
					Iter2.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter2.AbilityClass, Iter2.Level);
					if(!AttackAbilityQueues.Contains(Iter1.Key)) AttackAbilityQueues.Add(Iter1.Key);
					AttackAbilityQueues[Iter1.Key].Abilities.Add(Iter2);
				}
			}

			for(auto Iter : CharacterData.FallingAttackAbilities)
			{
				Iter.Value.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.Value.AbilityClass, Iter.Value.Level);
				FallingAttackAbilities.Add(Iter.Key, Iter.Value);
			}

			for(auto Iter : CharacterData.SkillAttackAbilities)
			{
				Iter.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.AbilityClass, Iter.Level);
				SkillAttackAbilities.Add(Iter.ID, Iter);
			}
		}
		else
		{
			AttackAbilityQueues = SaveData.AttackAbilityQueues;
			for(auto& Iter1 : AttackAbilityQueues)
			{
				for(auto& Iter2 : Iter1.Value.Abilities)
				{
					Iter2.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter2.AbilityClass, Iter2.Level);
				}
			}
		
			FallingAttackAbilities = SaveData.FallingAttackAbilities;
			for(auto& Iter : FallingAttackAbilities)
			{
				Iter.Value.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.Value.AbilityClass, Iter.Value.Level);
			}

			SkillAttackAbilities = SaveData.SkillAttackAbilities;
			for(auto& Iter : SkillAttackAbilities)
			{
				Iter.Value.AbilityHandle = AbilitySystem->K2_GiveAbility(Iter.Value.AbilityClass, Iter.Value.Level);
			}
		}

		Dialogue = SaveData.Dialogue;
	}
	if(PHASEC(InPhase, EPhase::Final))
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->Refresh();
		}
	}
}

FSaveData* ADWCharacter::ToData()
{
	static FDWCharacterSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FCharacterSaveData>();

	SaveData.TeamID = TeamID;
	SaveData.ControlMode = ControlMode;
	SaveData.WeaponGroup = WeaponGroup;
	SaveData.TalentPoint = TalentPoint;
	SaveData.TalentItems = TalentItems;

	SaveData.FallingAttackAbilities = FallingAttackAbilities;
	SaveData.AttackAbilityQueues = AttackAbilityQueues;
	SaveData.SkillAttackAbilities = SkillAttackAbilities;

	SaveData.Dialogue = Dialogue;

	return &SaveData;
}

void ADWCharacter::ResetData()
{
	Super::ResetData();

	SetMana(GetMaxMana());
	SetStamina(GetMaxStamina());
	SetHunger(100.f);
	SetThirst(100.f);
	SetOxygen(100.f);
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
			SwitchFiniteStateByClass<UDWCharacterState_Fly>();
			break;
		}
		case MOVE_Swimming:
		{
			if(!InCurrentState)
			{
				SwitchFiniteStateByClass<UAbilityCharacterState_Walk>();
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

void ADWCharacter::Kill(IAbilityVitalityInterface* InTarget)
{
	for(auto Iter : GetTeamMates<ADWCharacter>())
	{
		Iter->ModifyExp(InTarget->GetLevelA() * 10.f);
	}
	Super::Kill(InTarget);
}

void ADWCharacter::Revive(IAbilityVitalityInterface* InRescuer)
{
	Super::Revive(InRescuer);
}

void ADWCharacter::Dodge()
{
	SwitchFiniteStateByClass<UDWCharacterState_Dodge>();
}

void ADWCharacter::UnDodge()
{
	if(IsCurrentFiniteStateClass<UDWCharacterState_Dodge>())
	{
		SwitchFiniteState(nullptr);
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
	SwitchFiniteStateByClass<UDWCharacterState_Ride>({ InTarget });
}

void ADWCharacter::UnRide()
{
	if(IsCurrentFiniteStateClass<UDWCharacterState_Ride>())
	{
		SwitchFiniteState(nullptr);
	}
}

void ADWCharacter::Sleep(AVoxelInteractAuxiliary* InBed)
{
	SwitchFiniteStateByClass<UDWCharacterState_Sleep>({ InBed });
}

void ADWCharacter::UnSleep()
{
	if(IsCurrentFiniteStateClass<UDWCharacterState_Sleep>())
	{
		SwitchFiniteState(nullptr);
	}
}

void ADWCharacter::Aim()
{
	SwitchFiniteStateByClass<UDWCharacterState_Aim>();
}

void ADWCharacter::UnAim()
{
	if(IsCurrentFiniteStateClass<UDWCharacterState_Aim>())
	{
		SwitchFiniteState(nullptr);
	}
}

bool ADWCharacter::Attack(EDWWeaponPart InWeaponPart, int32 InAbilityIndex, const FSimpleDelegate& OnCompleted)
{
	const EDWWeaponType WeaponType = GetWeaponType(InWeaponPart);
	if(InAbilityIndex == -1) InAbilityIndex = GetAttackAbilityQueue(WeaponType).Index;

	if(!IsFalling())
	{
		const auto AbilityData = GetAttackAbility(WeaponType, InAbilityIndex);
		if(AbilityData.IsValid())
		{
			return SwitchFiniteStateByClass<UDWCharacterState_Attack>({ &AbilityData.AbilityHandle, (uint8)EDWCharacterAttackType::NormalAttack, (uint8)InWeaponPart, InAbilityIndex, &OnCompleted });
		}
	}
	else
	{
		return FallingAttack(InWeaponPart, OnCompleted);
	}
	return false;
}

bool ADWCharacter::FallingAttack(EDWWeaponPart InWeaponPart, const FSimpleDelegate& OnCompleted)
{
	const EDWWeaponType WeaponType = GetWeaponType(InWeaponPart);
	const auto AbilityData = GetFallingAttackAbility(WeaponType);
	if(AbilityData.IsValid())
	{
		if(!IsFalling()) Jump();

		return SwitchFiniteStateByClass<UDWCharacterState_Attack>({ &AbilityData.AbilityHandle, (uint8)EDWCharacterAttackType::FallingAttack, (uint8)InWeaponPart, &OnCompleted });
	}
	return false;
}

bool ADWCharacter::Skill(const FPrimaryAssetId& InSkillID)
{
	if(const auto SkillSlot = Inventory->GetSlotBySplitTypeAndItemID(ESlotSplitType::Skill, InSkillID))
	{
		return SkillSlot->ActiveItem();
	}
	return false;
}

bool ADWCharacter::Skill(int32 InSkillIndex)
{
	if(const auto SkillSlot = Inventory->GetSlotBySplitTypeAndIndex(ESlotSplitType::Skill, InSkillIndex))
	{
		return SkillSlot->ActiveItem();
	}
	return false;
}

bool ADWCharacter::Skill(EDWSkillType InSkillType, int32 InAbilityIndex)
{
	TArray<UAbilityInventorySlotBase*> ItemSlots;
	for (auto Iter : Inventory->GetSlotsBySplitType(ESlotSplitType::Skill))
	{
		if(!Iter->IsEmpty() && Iter->GetItem().GetData<UDWSkillData>().SkillType == InSkillType)
		{
			ItemSlots.Add(Iter);
		}
	}
	if(InAbilityIndex == -1) InAbilityIndex = FMath::RandRange(0, ItemSlots.Num() -1);

	if(ItemSlots.IsValidIndex(InAbilityIndex))
	{
		return ItemSlots[InAbilityIndex]->ActiveItem();
	}
	return false;
}

bool ADWCharacter::SkillAttack(const FAbilityItem& InAbilityItem, const FSimpleDelegate& OnCompleted/* = nullptr*/)
{
	const auto AbilityData = GetSkillAttackAbility(InAbilityItem.ID);
	if(AbilityData.IsValid())
	{
		return SwitchFiniteStateByClass<UDWCharacterState_Attack>({ &AbilityData.AbilityHandle, (uint8)EDWCharacterAttackType::SkillAttack, (uint8)InAbilityItem.GetData<UDWSkillData>().WeaponPart, &InAbilityItem, &OnCompleted });
	}
	return false;
}

void ADWCharacter::UnAttack()
{
	if(IsCurrentFiniteStateClass<UDWCharacterState_Attack>())
	{
		SwitchFiniteState(nullptr);
	}
}

void ADWCharacter::Defend()
{
	SwitchFiniteStateByClass<UDWCharacterState_Defend>();
}

void ADWCharacter::UnDefend()
{
	if(IsCurrentFiniteStateClass<UDWCharacterState_Defend>())
	{
		SwitchFiniteState(nullptr);
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
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);

	if((!IsCurrent() && !IsPlayer()) || UDWSettingModule::Get().IsAutoJump(true))
	{
		FHitResult HitResult;
		if(RaycastStep(HitResult))
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
	GetCharacterMovement()->RotationRate = FRotator(0.f, GetRotationSpeed() * (IsSprinting() ? 1.5f : 1.f) * RotationRate, 0.f);
}

bool ADWCharacter::RaycastStep(FHitResult& OutHitResult)
{
	const FVector RayStart = GetActorLocation() + FVector::DownVector * (GetHalfHeight() - GetCharacterMovement()->MaxStepHeight);
	const FVector RayEnd = RayStart + GetMoveDirection(true) * (GetRadius() + 100.f * FMath::Lerp(0.35f, 1.25f, GetMoveVelocity(true).Size() / 500.f));
	return UKismetSystemLibrary::LineTraceSingle(this, RayStart, RayEnd, USceneModuleStatics::GetTraceMapping(FName("Step")).GetTraceType(), false, {}, EDrawDebugTrace::None, OutHitResult, true);
}

bool ADWCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	return Super::CanInteract(InInteractAction, InInteractionAgent);
}

void ADWCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassive);

	if(!bPassive)
	{
		switch (InInteractAction)
		{
			case EInteractAction::Interact:
			{
				if(AVoxelInteractAuxiliary* InteractionAgent = Cast<AVoxelInteractAuxiliary>(InInteractionAgent))
				{
					switch(InteractionAgent->GetVoxelItem().GetVoxelType())
					{
						case EVoxelType::Bed:
						{
							Sleep(InteractionAgent);
							break;
						}
						default: break;
					}
				}
				break;
			}
			case EInteractAction::UnInteract:
			{
				if(AVoxelInteractAuxiliary* InteractionAgent = Cast<AVoxelInteractAuxiliary>(InInteractionAgent))
				{
					switch(InteractionAgent->GetVoxelItem().GetVoxelType())
					{
						case EVoxelType::Bed:
						{
							UnSleep();
							break;
						}
						default: break;
					}
				}
				break;
			}
			default: break;
		}
	}
}

void ADWCharacter::OnAdditionItem(const FAbilityItem& InItem)
{
	Super::OnAdditionItem(InItem);
}

void ADWCharacter::OnPreChangeItem(const FAbilityItem& InOldItem)
{
	Super::OnPreChangeItem(InOldItem);
}

void ADWCharacter::OnChangeItem(const FAbilityItem& InNewItem)
{
	Super::OnChangeItem(InNewItem);
}

void ADWCharacter::OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess)
{
	Super::OnActiveItem(InItem, bPassive, bSuccess);

	if(!bPassive && bSuccess)
	{
		if(InItem.GetType() == EAbilityItemType::Prop)
		{
			DoAction(GameplayTags::Ability_Character_Action_Use);
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

void ADWCharacter::OnSelectItem(const FAbilityItem& InItem)
{
	Super::OnSelectItem(InItem);
}

void ADWCharacter::OnAuxiliaryItem(const FAbilityItem& InItem)
{
	Super::OnAuxiliaryItem(InItem);
}

void ADWCharacter::OnPickUp(AAbilityPickUpBase* InPickUp)
{
	Super::OnPickUp(InPickUp);
}

bool ADWCharacter::OnGenerateVoxel(EInputInteractEvent InInteractEvent, const FVoxelHitResult& InHitResult)
{
	switch(InInteractEvent)
	{
		case EInputInteractEvent::Started:
		{
			return IVoxelAgentInterface::OnGenerateVoxel(InInteractEvent, InHitResult);
		}
		case EInputInteractEvent::Triggered:
		{
			return IVoxelAgentInterface::OnGenerateVoxel(InInteractEvent, InHitResult);
		}
		case EInputInteractEvent::Completed:
		{
			FItemQueryData ItemQueryData = Inventory->QueryItemByRange(EItemQueryType::Remove, GenerateVoxelItem.ID, -1);
			if(!ItemQueryData.IsValid() || !DoAction(GameplayTags::Ability_Character_Action_Generate))
			{
				bCanGenerateVoxel = false;
			}
			if(IVoxelAgentInterface::OnGenerateVoxel(InInteractEvent, InHitResult))
			{
				Inventory->RemoveItemByQueryData(ItemQueryData);
				if(IsPlayer())
				{
					UAchievementModuleStatics::UnlockAchievement(FName("FirstGenerateVoxel"));
				}
				return true;
			}
			break;
		}
	}
	return false;
}

bool ADWCharacter::OnDestroyVoxel(EInputInteractEvent InInteractEvent, const FVoxelHitResult& InHitResult)
{
	switch(InInteractEvent)
	{
		case EInputInteractEvent::Started:
		{
			if(DoAction(GameplayTags::Ability_Character_Action_Destroy))
			{
				return IVoxelAgentInterface::OnDestroyVoxel(InInteractEvent, InHitResult);
			}
			break;
		}
		case EInputInteractEvent::Triggered:
		{
			if(DestroyVoxelItem.IsValid())
			{
				if(IsPlayer())
				{
					UWidgetModuleStatics::OpenUserWidget<UWidgetProgressBox>({ DestroyVoxelItem.GetVoxelData().Name, FText::FromString(TEXT("破坏体素")) });
				}
				DoAction(GameplayTags::Ability_Character_Action_Destroy);
				if(IsExhausted())
				{
					OnDestroyVoxel(EInputInteractEvent::Completed, InHitResult);
					break;
				}
				if(IsPlayer() && UWidgetModuleStatics::GetUserWidget<UWidgetProgressBox>())
				{
					UWidgetModuleStatics::GetUserWidget<UWidgetProgressBox>()->SetProgress(1.f - DestroyVoxelItem.Durability);
				}
			}
			if(IVoxelAgentInterface::OnDestroyVoxel(InInteractEvent, InHitResult))
			{
				if(IsPlayer())
				{
					UWidgetModuleStatics::CloseUserWidget<UWidgetProgressBox>();
					UAchievementModuleStatics::UnlockAchievement(FName("FirstDestroyVoxel"));
				}
				return true;
			}
			break;
		}
		case EInputInteractEvent::Completed:
		{
			if(DestroyVoxelItem.IsValid())
			{
				StopAction(GameplayTags::Ability_Character_Action_Destroy);
				if(IsPlayer())
				{
					UWidgetModuleStatics::CloseUserWidget<UWidgetProgressBox>();
				}
			}
			return IVoxelAgentInterface::OnDestroyVoxel(InInteractEvent, InHitResult);
		}
	}
	return false;
}

void ADWCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	if(InAttributeChangeData.Attribute == GetExpAttribute() || InAttributeChangeData.Attribute == GetMaxExpAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>() && UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this))
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this)->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetHealthAttribute() || InAttributeChangeData.Attribute == GetMaxHealthAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetHealthPercent(GetHealth(), GetMaxHealth());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>() && UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this))
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this)->SetHealthPercent(GetHealth(), GetMaxHealth());
		}
	}
	else if(InAttributeChangeData.Attribute == GetManaAttribute() || InAttributeChangeData.Attribute == GetMaxManaAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetManaPercent(GetMana(), GetMaxMana());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>() && UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this))
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this)->SetManaPercent(GetMana(), GetMaxMana());
		}
	}
	else if(InAttributeChangeData.Attribute == GetStaminaAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>() && UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this))
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this)->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
	}
	else if(InAttributeChangeData.Attribute == GetMaxStaminaAttribute())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>() && UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this))
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->GetTeamMateHeadBoxWidget(this)->SetStaminaPercent(GetStamina(), GetMaxStamina());
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

void ADWCharacter::OnWorldModeChanged(UObject* InSender, UEventHandle_VoxelWorldModeChanged* InEventHandle)
{
	if(IsCurrentFiniteStateClass<UDWCharacterState_Spawn>()) return;
	
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

void ADWCharacter::HandleDamage(const FGameplayAttribute& DamageAttribute, float DamageValue, float DefendValue, bool bHasCrited, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleDamage(DamageAttribute, DamageValue, DefendValue, bHasCrited, HitResult, SourceTags, SourceActor);

	if(!IsDead())
	{
		if(!IsDefending())
		{
			DoAction(GameplayTags::Ability_Character_Action_GetHit);
		}
		else
		{
			DoAction(GameplayTags::Ability_Character_Action_DefendBlock);
		}

		ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor);

		if(SourceCharacter && SourceCharacter != this)
		{
			if(UDWAIBlackboard* Blackboard = GetBlackboard<UDWAIBlackboard>())
			{
				Blackboard->SetTargetAgent(SourceCharacter);
				Blackboard->SetIsExcessived(true);
			}
			
			ADWCharacter* PlayerCharacter = nullptr;
			ADWCharacter* TargetCharacter = nullptr;
			if(IsPlayer())
			{
				PlayerCharacter = this;
				TargetCharacter = SourceCharacter;
			}
			else if(SourceCharacter->IsPlayer())
			{
				PlayerCharacter = SourceCharacter;
				TargetCharacter = this;
			}
			if(PlayerCharacter && TargetCharacter)
			{
				for(auto Iter : PlayerCharacter->GetTeamMates<ADWCharacter>())
				{
					if(UDWAIBlackboard* Blackboard = Iter->GetBlackboard<UDWAIBlackboard>())
					{
						Blackboard->SetTargetAgent(TargetCharacter);
						Blackboard->SetIsExcessived(true);
					}
				}
			}
		}
	}
}

void ADWCharacter::HandleRecovery(const FGameplayAttribute& RecoveryAttribute, float RecoveryValue, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleRecovery(RecoveryAttribute, RecoveryValue, HitResult, SourceTags, SourceActor);

	if(RecoveryAttribute == GetManaRecoveryAttribute())
	{
		ModifyMana(RecoveryValue);
	}
	else if(RecoveryAttribute == GetStaminaRecoveryAttribute())
	{
		ModifyStamina(RecoveryValue);
	}
	else if(RecoveryAttribute == GetHungerRecoveryAttribute())
	{
		ModifyHunger(RecoveryValue);
	}
	else if(RecoveryAttribute == GetThirstRecoveryAttribute())
	{
		ModifyThirst(RecoveryValue);
	}
	else if(RecoveryAttribute == GetOxygenRecoveryAttribute())
	{
		ModifyOxygen(RecoveryValue);
	}
}

void ADWCharacter::HandleInterrupt(const FGameplayAttribute& InterruptAttribute, float InterruptDuration, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleInterrupt(InterruptAttribute, InterruptDuration, HitResult, SourceTags, SourceActor);

	if(InterruptAttribute == GetInterruptAttribute())
	{
		if(InterruptDuration > 0.f)
		{
			Interrupt(InterruptDuration * (1.f - GetToughnessRate()));
		}
	}
}

bool ADWCharacter::IsTargetAble_Implementation(APawn* InPlayerPawn) const
{
	return Super::IsTargetAble_Implementation(InPlayerPawn) && FSM->GetStateByClass<UDWCharacterState_Ride>()->GetOwnerRider() != InPlayerPawn;
}

bool ADWCharacter::IsLookAtAble_Implementation(AActor* InLookerActor) const
{
	return Super::IsLookAtAble_Implementation(InLookerActor) && FSM->GetStateByClass<UDWCharacterState_Ride>()->GetOwnerRider() != InLookerActor;
}

bool ADWCharacter::CanLookAtTarget()
{
	return Super::CanLookAtTarget() && !IsDodging();
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

bool ADWCharacter::IsEnemy(IAbilityPawnInterface* InTarget) const
{
	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(InTarget);

	if(!TargetCharacter) return false;

	if(IsTeamMate(TargetCharacter)) return false;

	switch (GetNature())
	{
		case EDWCharacterNature::NPC:
		case EDWCharacterNature::AIFriendly:
		{
			return false;
		}
		case EDWCharacterNature::AINeutral:
		{
			if(!GetBlackboard<UDWAIBlackboard>()->GetIsExcessived())
			{
				return false;
			}
			break;
		}
		default:
		{
			switch (TargetCharacter->GetNature())
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
	
	return Super::IsEnemy(InTarget);
}

bool ADWCharacter::IsEnemyN(ADWCharacter* InTarget)
{
	return IsEnemy(InTarget);
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

bool ADWCharacter::IsSleeping() const
{
	return AbilitySystem->HasMatchingGameplayTag(GameplayTags::State_Character_Sleeping);
}

void ADWCharacter::SetControlMode(EDWCharacterControlMode InControlMode)
{
	ControlMode = InControlMode;

	switch (ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			for(auto Iter : GetWeapons())
			{
				Execute_SetActorVisible(Iter, Execute_IsVisible(this));
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			for(auto Iter : GetWeapons())
			{
				Execute_SetActorVisible(Iter, false);
			}
			break;
		}
	}
}

void ADWCharacter::SetWeaponGroup(EDWWeaponGroup InWeaponGroup)
{
	for(auto Iter : Inventory->GetSlotsBySplitType(ESlotSplitType::Equip))
	{
		if(UDWInventoryEquipSlot* EquipSlot = Cast<UDWInventoryEquipSlot>(Iter))
		{
			if(UDWCommonStatics::GetWeaponPartsByGroup(WeaponGroup).Contains(EquipSlot->GetEquipPart()))
			{
				EquipSlot->OnItemPreChange(EquipSlot->GetItem(), true);
			}
		}
	}

	WeaponGroup = InWeaponGroup;

	for(auto Iter : Inventory->GetSlotsBySplitType(ESlotSplitType::Equip))
	{
		if(UDWInventoryEquipSlot* EquipSlot = Cast<UDWInventoryEquipSlot>(Iter))
		{
			if(UDWCommonStatics::GetWeaponPartsByGroup(WeaponGroup).Contains(EquipSlot->GetEquipPart()))
			{
				EquipSlot->OnItemChanged(EquipSlot->GetItem(), true);
			}
			EquipSlot->Refresh();
		}
	}
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
	const int32 _level = Level;
	
	if(!Super::SetLevelA(InLevel)) return false;

	TalentPoint += Level - _level;

	if(GetCharacterHPWidget())
	{
		GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}

	if(!IsPlayer())
	{
		if(GetCharacterHPWidget())
		{
			GetCharacterHPWidget()->Refresh();
		}
	}
	
	return true;
}

void ADWCharacter::ModifyExp(float InDeltaValue)
{
	Super::ModifyExp(InDeltaValue);
}

void ADWCharacter::SetTeamID(FName InTeamID)
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(InTeamID.IsNone() && IsTeamMate(PlayerCharacter))
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->RemoveTeamMate(this);
		}
	}
	
	TeamID = InTeamID;

	if(GetCharacterHPWidget())
	{
		GetCharacterHPWidget()->SetHeadInfo(GetHeadInfo());
	}

	if(!InTeamID.IsNone() && IsTeamMate(PlayerCharacter))
	{
		if(UDWAIBlackboard* Blackboard = GetBlackboard<UDWAIBlackboard>())
		{
			Blackboard->SetTargetAgent(PlayerCharacter);
			Blackboard->SetIsExcessived(false);
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->AddTeamMate(this);
		}
		RefreshAttributes();
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

ADWEquip* ADWCharacter::GetEquip(EDWEquipPart InEquipPart) const
{
	if(Equips.Contains(InEquipPart))
	{
		return Equips[InEquipPart];
	}
	return nullptr;
}

TArray<ADWEquip*> ADWCharacter::GetEquips() const
{
	TArray<ADWEquip*> ReturnValues;
	Equips.GenerateValueArray(ReturnValues);
	return ReturnValues;
}

ADWEquipWeapon* ADWCharacter::GetWeapon(EDWWeaponPart InWeaponPart) const
{
	for(auto Iter : GetWeapons())
	{
		const auto& WeaponData = Iter->GetItemData<UDWEquipWeaponData>();
		if(InWeaponPart == EDWWeaponPart::None || WeaponData.EquipPart == (EDWEquipPart)InWeaponPart)
		{
			return Iter;
		}
	}
	return nullptr;
}

TArray<ADWEquipWeapon*> ADWCharacter::GetWeapons() const
{
	TArray<ADWEquipWeapon*> ReturnValues;
	const TArray WeaponParts = { EDWEquipPart::Primary, EDWEquipPart::Secondary };
	ITER_ARRAY(WeaponParts, WeaponPart,
		if(ADWEquipWeapon* Weapon = GetEquip<ADWEquipWeapon>(WeaponPart))
		{
			ReturnValues.Add(Weapon);
		}
	)
	return ReturnValues;
}

EDWWeaponType ADWCharacter::GetWeaponType(EDWWeaponPart InWeaponPart) const
{
	if(ADWEquipWeapon* Weapon = GetWeapon(InWeaponPart))
	{
		return Weapon->GetItemData<UDWEquipWeaponData>().WeaponType;
	}
	return EDWWeaponType::None;
}

bool ADWCharacter::CheckWeaponType(EDWWeaponPart InWeaponPart, EDWWeaponType InWeaponType) const
{
	if(InWeaponType == EDWWeaponType::None) return true;
	
	for(auto Iter : GetWeapons())
	{
		const auto& WeaponData = Iter->GetItemData<UDWEquipWeaponData>();
		if((InWeaponPart == EDWWeaponPart::None || WeaponData.EquipPart == (EDWEquipPart)InWeaponPart) && WeaponData.WeaponType == InWeaponType)
		{
			return true;
		}
	}
	return false;
}

TSubclassOf<AAbilityProjectileBase> ADWCharacter::GetWeaponProjectile(EDWWeaponPart InWeaponPart) const
{
	if(ADWEquipWeapon* Weapon = GetWeapon(InWeaponPart))
	{
		return Weapon->GetItemData<UDWEquipWeaponData>().ProjectileClass;
	}
	return nullptr;
}

bool ADWCharacter::HasAttackAbility(EDWWeaponType InWeaponType, int32 InAbilityIndex) const
{
	return GetAttackAbilityQueue(InWeaponType).Abilities.IsValidIndex(InAbilityIndex);
}

FDWCharacterAttackAbilityData ADWCharacter::GetAttackAbility(EDWWeaponType InWeaponType, int32 InAbilityIndex) const
{
	if(InAbilityIndex == -1) InAbilityIndex = GetAttackAbilityIndex(InWeaponType);
	if(HasAttackAbility(InWeaponType, InAbilityIndex))
	{
		return GetAttackAbilities(InWeaponType)[InAbilityIndex];
	}
	return FDWCharacterAttackAbilityData();
}

FDWCharacterAttackAbilityQueue ADWCharacter::GetAttackAbilityQueue(EDWWeaponType InWeaponType) const
{
	return const_cast<ADWCharacter*>(this)->GetAttackAbilityQueue(InWeaponType);
}

FDWCharacterAttackAbilityQueue& ADWCharacter::GetAttackAbilityQueue(EDWWeaponType InWeaponType)
{
	if(AttackAbilityQueues.Contains(InWeaponType))
	{
		return AttackAbilityQueues[InWeaponType];
	}
	static FDWCharacterAttackAbilityQueue Empty;
	return Empty;
}

int32 ADWCharacter::GetAttackAbilityIndex(EDWWeaponType InWeaponType) const
{
	return GetAttackAbilityQueue(InWeaponType).Index;
}

TArray<FDWCharacterAttackAbilityData> ADWCharacter::GetAttackAbilities(EDWWeaponType InWeaponType) const
{
	return GetAttackAbilityQueue(InWeaponType).Abilities;
}

bool ADWCharacter::HasFallingAttackAbility(EDWWeaponType InWeaponType) const
{
	return FallingAttackAbilities.Contains(InWeaponType);
}

FDWCharacterFallingAttackAbilityData ADWCharacter::GetFallingAttackAbility(EDWWeaponType InWeaponType)
{
	if(HasFallingAttackAbility(InWeaponType))
	{
		return FallingAttackAbilities[InWeaponType];
	}
	return FDWCharacterFallingAttackAbilityData();
}

bool ADWCharacter::HasSkillAttackAbility(const FPrimaryAssetId& InSkillID, bool bNeedSlotted) const
{
	if(SkillAttackAbilities.Contains(InSkillID))
	{
		if(!bNeedSlotted) return true;
		return Inventory->QueryItemBySplitType(EItemQueryType::Get, InSkillID, ESlotSplitType::Skill).IsValid();
	}
	return false;
}

FDWCharacterSkillAttackAbilityData ADWCharacter::GetSkillAttackAbility(const FPrimaryAssetId& InSkillID, bool bNeedSlotted)
{
	if(HasSkillAttackAbility(InSkillID, bNeedSlotted))
	{
		return SkillAttackAbilities[InSkillID];
	}
	return FDWCharacterSkillAttackAbilityData();
}

UDWCharacterPart* ADWCharacter::GetCharacterPart(EDWCharacterPart InCharacterPart) const
{
	TArray<UDWCharacterPart*> Parts;
	GetComponents(Parts);
	for(auto Iter : Parts)
	{
		if(Iter->GetCharacterPart() == InCharacterPart)
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
