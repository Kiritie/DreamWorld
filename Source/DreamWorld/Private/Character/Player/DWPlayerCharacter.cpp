// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Character/Player/DWPlayerCharacter.h"

#include "Achievement/AchievementModuleStatics.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"
#include "Ability/AbilityModuleTypes.h"
#include "Ability/Character/AbilityCharacterInventoryBase.h"
#include "Ability/Character/States/AbilityCharacterState_Crouch.h"
#include "Ability/Character/States/AbilityCharacterState_Fall.h"
#include "Ability/Character/States/AbilityCharacterState_Interrupt.h"
#include "Ability/Character/States/AbilityCharacterState_Jump.h"
#include "Ability/Character/States/AbilityCharacterState_Static.h"
#include "Ability/Character/States/AbilityCharacterState_Swim.h"
#include "Ability/Character/States/AbilityCharacterState_Walk.h"
#include "Camera/CameraModuleStatics.h"
#include "Character/Player/States/DWPlayerCharacterState_Death.h"
#include "Character/Player/States/DWPlayerCharacterState_Spawn.h"
#include "Character/States/DWCharacterState_Float.h"
#include "Character/States/DWCharacterState_Fly.h"
#include "Character/States/DWCharacterState_Ride.h"
#include "FSM/Components/FSMComponent.h"
#include "Character/DWCharacterData.h"
#include "Character/Human/States/DWHumanCharacterState_Defend.h"
#include "Character/Player/States/DWPlayerCharacterState_Aim.h"
#include "Character/Player/States/DWPlayerCharacterState_Sleep.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Character/States/DWCharacterState_Climb.h"
#include "Character/States/DWCharacterState_Dodge.h"
#include "Common/CommonStatics.h"
#include "Common/Looking/LookingComponent.h"
#include "Common/Targeting/TargetingComponent.h"
#include "Vitality/DWVitality.h"
#include "Voxel/Voxels/VoxelInteract.h"
#include "Voxel/Voxels/Auxiliary/VoxelAuxiliary.h"
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"
#include "Widget/Context/WidgetContextBox.h"
#include "Widget/Generate/WidgetGeneratePanel.h"
#include "Widget/Head/WidgetHeadBox.h"
#include "Widget/Interaction/WidgetInteractionBox.h"
#include "Widget/Inventory/WidgetInventoryBox.h"
#include "Widget/Message/WidgetMessageBox.h"

//////////////////////////////////////////////////////////////////////////
// ADWPlayerCharacter

ADWPlayerCharacter::ADWPlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	AIControllerClass = nullptr;
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(69.f);
	GetCapsuleComponent()->SetCapsuleRadius(24.f);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -70));

	Looking->LookingMaxDistance = 1500.f;

	Targeting = CreateDefaultSubobject<UTargetingComponent>(FName("Targeting"));
	Targeting->bShouldControlRotation = false;
	Targeting->TargetableActors = ADWCharacter::StaticClass();
	Targeting->TargetableCollisionChannel = ECC_GameTraceChannel1;
	Targeting->LockedOnWidgetParentSocket = FName("LockPoint");
	Targeting->MinimumDistanceToEnable = 1500.f;
	Targeting->RotatingSmoothnessSpeed = 20.f;
	Targeting->PitchMin = -20.f;
	Targeting->PitchMax = -10.f;
	Targeting->OnTargetLockOn.AddDynamic(this, &ADWPlayerCharacter::OnTargetLockedOn);
	Targeting->OnTargetLockOff.AddDynamic(this, &ADWPlayerCharacter::OnTargetLockedOff);
	Targeting->OnTargetSetRotation.AddDynamic(this, &ADWPlayerCharacter::OnTargetSetRotation);

	PreviewCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("PreviewCapture"));
	PreviewCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	PreviewCapture->OrthoWidth = 100.f;
	PreviewCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	PreviewCapture->SetupAttachment(RootComponent);
	PreviewCapture->SetRelativeLocationAndRotation(FVector(100.f, 0.f, 0.f), FRotator(0.f, 180.f, 0.f));

	// FSM->bShowDebugMessage = true;
	FSM->DefaultState = UDWPlayerCharacterState_Spawn::StaticClass();
	FSM->FinalState = UDWPlayerCharacterState_Death::StaticClass();
	
	FSM->States.Empty();
	FSM->States.Add(UDWPlayerCharacterState_Aim::StaticClass());
	FSM->States.Add(UDWCharacterState_Attack::StaticClass());
	FSM->States.Add(UDWCharacterState_Climb::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Crouch::StaticClass());
	FSM->States.Add(UDWPlayerCharacterState_Death::StaticClass());
	FSM->States.Add(UDWPlayerCharacterState_Spawn::StaticClass());
	FSM->States.Add(UDWHumanCharacterState_Defend::StaticClass());
	FSM->States.Add(UDWCharacterState_Dodge::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Fall::StaticClass());
	FSM->States.Add(UDWCharacterState_Float::StaticClass());
	FSM->States.Add(UDWCharacterState_Fly::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Interrupt::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Jump::StaticClass());
	FSM->States.Add(UDWCharacterState_Ride::StaticClass());
	FSM->States.Add(UDWPlayerCharacterState_Sleep::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Static::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Swim::StaticClass());
	FSM->States.Add(UAbilityCharacterState_Walk::StaticClass());
}

void ADWPlayerCharacter::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();

	Targeting->LocationOffset = FVector(0.f, Execute_GetCameraOffset(this).Y, 0.f);

	PreviewCapture->ShowOnlyActors.Add(this);
}

void ADWPlayerCharacter::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);
}

void ADWPlayerCharacter::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
}

void ADWPlayerCharacter::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWPlayerSaveData>();

	if(PHASEC(InPhase, EPhase::PrimaryAndLesser))
	{
		if(!SaveData.InventoryData.IsSaved())
		{
			SaveData.InitInventoryData();
		}
	}

	if(PHASEC(InPhase, EPhase::All))
	{
		SetBodyColor(SaveData.BodyColorIndex);
		SetCapeColor(SaveData.CapeColorIndex);
	}

	if(PHASEC(InPhase, EPhase::Final))
	{
		RefreshAttributes();
	}

	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWPlayerCharacter::ToData()
{
	static FDWPlayerSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FDWCharacterSaveData>();
	
	SaveData.BodyColorIndex = GetBodyColor();
	SaveData.CapeColorIndex = GetCapeColor();
	
	SaveData.CameraRotation = UCameraModuleStatics::GetCameraRotation();
	SaveData.CameraDistance = UCameraModuleStatics::GetCameraDistance();

	return &SaveData;
}

void ADWPlayerCharacter::Death(IAbilityVitalityInterface* InKiller /* = nullptr */)
{
	Super::Death(InKiller);

	if(InKiller)
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(InKiller != this ? FString::Printf(TEXT("你被 %s 杀死了！"), *InKiller->GetNameA().ToString()) : TEXT("你自杀了！"));
	}
}

void ADWPlayerCharacter::Kill(IAbilityVitalityInterface* InTarget)
{
	Super::Kill(InTarget);
	
	if(InTarget != this)
	{
		if(Cast<ADWCharacter>(InTarget))
		{
			UAchievementModuleStatics::UnlockAchievement(FName("FirstKillMonster"));
		}
		else if(Cast<ADWVitality>(InTarget))
		{
			UAchievementModuleStatics::UnlockAchievement(FName("FirstKillVitality"));
		}
	}
}

void ADWPlayerCharacter::ChangeHand()
{
	auto AuxiliarySlot = Inventory->GetSlotBySplitTypeAndIndex(ESlotSplitType::Auxiliary, 0);
	if(AuxiliarySlot && Inventory->GetSelectedSlot(ESlotSplitType::Shortcut))
	{
		AuxiliarySlot->Replace(Inventory->GetSelectedSlot(ESlotSplitType::Shortcut));
	}
}

bool ADWPlayerCharacter::CanLookAtTarget()
{
	return Super::CanLookAtTarget() && (IsAttacking() || IsDefending() || IsAiming());
}

void ADWPlayerCharacter::OnTargetLookAtOn(AActor* InTargetActor)
{
	Super::OnTargetLookAtOn(InTargetActor);
}

void ADWPlayerCharacter::OnTargetLookAtOff(AActor* InTargetActor)
{
	Super::OnTargetLookAtOff(InTargetActor);
}

bool ADWPlayerCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	return Super::CanInteract(InInteractAction, InInteractionAgent);
}

void ADWPlayerCharacter::OnEnterInteract(IInteractionAgentInterface* InInteractionAgent)
{
	Super::OnEnterInteract(InInteractionAgent);

	UWidgetModuleStatics::OpenUserWidget<UWidgetInteractionBox>({ Cast<UObject>(InInteractionAgent) });
}

void ADWPlayerCharacter::OnLeaveInteract(IInteractionAgentInterface* InInteractionAgent)
{
	Super::OnLeaveInteract(InInteractionAgent);

	UWidgetModuleStatics::CloseUserWidget<UWidgetInteractionBox>();
}

void ADWPlayerCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassive);

	if(!bPassive)
	{
		switch(InInteractAction)
		{
			case (EInteractAction)EVoxelInteractAction::Open:
			{
				if(AVoxelInteractAuxiliary* InteractionAgent = Cast<AVoxelInteractAuxiliary>(InInteractionAgent))
				{
					switch(InteractionAgent->GetVoxelItem().GetVoxelType())
					{
						case EVoxelType::Chest:
						{
							if(UWidgetInventoryBox* InventoryBox = UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBox>())
							{
								FDelegateHandle DelegateHandle = InventoryBox->OnClosed.AddLambda([this, InteractionAgent, DelegateHandle, InventoryBox](bool bInstant)
								{
									InventoryBox->OnClosed.Remove(DelegateHandle);
									DoInteract((EInteractAction)EVoxelInteractAction::Close, InteractionAgent);
								});
								InventoryBox->Open({ InteractionAgent });
							}
							break;
						}
						case EVoxelType::Furnace:
						case EVoxelType::Crafting_Table:
						{
							if(UWidgetGeneratePanel* GeneratePanel = UWidgetModuleStatics::GetUserWidget<UWidgetGeneratePanel>())
							{
								FDelegateHandle DelegateHandle = GeneratePanel->OnClosed.AddLambda([this, InteractionAgent, DelegateHandle, GeneratePanel](bool bInstant)
								{
									GeneratePanel->OnClosed.Remove(DelegateHandle);
									DoInteract((EInteractAction)EVoxelInteractAction::Close, InteractionAgent);
								});
								GeneratePanel->Open({ InteractionAgent });
							}
							break;
						}
						default: break;
					}
				}
				break;
			}
			case (EInteractAction)EVoxelInteractAction::Close:
			{
				if(!IsOverlapping(InInteractionAgent))
				{
					SetInteractingAgent(nullptr, true);
				}
				break;
			}
			default: break;
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInteractionBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInteractionBox>()->Refresh();
		}
	}
}

bool ADWPlayerCharacter::OnInteractVoxel(EInputInteractAction InInteractAction, EInputInteractEvent InInteractEvent, const FVoxelHitResult& InHitResult)
{
	return Super::OnInteractVoxel(InInteractAction, InInteractEvent, InHitResult);
}

void ADWPlayerCharacter::MoveForward_Implementation(float InValue)
{
	FVector Direction;
	const FRotator Rotation = GetControlRotation();
	if(IsFlying() || IsSwimming() || IsFloating())
	{
		Direction = Rotation.Vector();
	}
	else
	{
		const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);
		Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	}
	AddMovementInput(Direction, InValue);
}

void ADWPlayerCharacter::MoveRight_Implementation(float InValue)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, InValue);
}

void ADWPlayerCharacter::MoveUp_Implementation(float InValue)
{
	if(IsFlying() || IsSwimming() || IsFloating())
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(FVector(Direction.X * 0.2f, Direction.Y * 0.2f, 0.5f * InValue), 1.f);
	}
}

void ADWPlayerCharacter::OnAdditionItem(const FAbilityItem& InItem)
{
	Super::OnAdditionItem(InItem);

	if(UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>())
	{
		if(InItem.GetType() != EAbilityItemType::Misc)
		{
			if(InItem.Count > 0)
			{
				UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("获得: %s × %d"), *InItem.GetData().Name.ToString(), InItem.Count));
			}
			else
			{
				UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("无法再获得: %s"), *InItem.GetData().Name.ToString()));
			}
		}
	}
}

void ADWPlayerCharacter::OnChangeItem(const FAbilityItem& InNewItem)
{
	Super::OnChangeItem(InNewItem);
}

void ADWPlayerCharacter::OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess)
{
	Super::OnActiveItem(InItem, bPassive, bSuccess);

	if(!bPassive && !bSuccess)
	{
		UWidgetModuleStatics::OpenUserWidget<UWidgetMessageBox>({ FText::FromString(FString::Printf(TEXT("暂时无法使用该%s！"), *UCommonStatics::GetEnumDisplayNameByValue(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)InItem.GetType()).ToString())) });
	}
}

void ADWPlayerCharacter::OnDeactiveItem(const FAbilityItem& InItem, bool bPassive)
{
	Super::OnDeactiveItem(InItem, bPassive);
}

void ADWPlayerCharacter::OnSelectItem(const FAbilityItem& InItem)
{
	Super::OnSelectItem(InItem);
}

void ADWPlayerCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;
	
	if(InAttributeChangeData.Attribute == GetHealthAttribute() || InAttributeChangeData.Attribute == GetMaxHealthAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->SetHealthPercent(GetHealth(), GetMaxHealth());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHealthInfo(FString::Printf(TEXT("%d/%d"), (int32)GetHealth(), (int32)GetMaxHealth()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetExpAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>())
		{
			if(DeltaValue > 0.f)
			{
				UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("获得: %d点经验值"), (int32)DeltaValue));
			}
			if(InAttributeChangeData.NewValue >= GetMaxExp())
			{
				UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("等级提升！")));
			}
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->SetHeadInfo(GetHeadInfo());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetMaxExpAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->SetHeadInfo(GetHeadInfo());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetManaAttribute() || InAttributeChangeData.Attribute == GetMaxManaAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->SetManaPercent(GetMana(), GetMaxMana());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetManaInfo(FString::Printf(TEXT("%d/%d"), (int32)GetMana(), (int32)GetMaxMana()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetStaminaAttribute() || InAttributeChangeData.Attribute == GetMaxStaminaAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetStaminaInfo(FString::Printf(TEXT("%d/%d"), (int32)GetStamina(), (int32)GetMaxStamina()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetMoveSpeedAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetMoveSpeed(UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue, 2));
		}
	}
	else if(InAttributeChangeData.Attribute == GetSwimSpeedAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetSwimSpeed(UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue, 2));
		}
	}
	else if(InAttributeChangeData.Attribute == GetFlySpeedAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetFlySpeed(UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue, 2));
		}
	}
	else if(InAttributeChangeData.Attribute == GetRotationSpeedAttribute())
	{
		GetCharacterMovement()->RotationRate = FRotator(0, InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1), 0) * RotationRate;
	}
	else if(InAttributeChangeData.Attribute == GetAttackForceAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetAttackForce(UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue, 2));
		}
	}
	else if(InAttributeChangeData.Attribute == GetRepulseForceAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetRepulseForce(UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue, 2));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttackSpeedAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetAttackSpeed(UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue, 2));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttackCritRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetAttackCritRate(FString::Printf(TEXT("%s%%"), *UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue * 100.f, 2)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttackStealRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetAttackStealRate(FString::Printf(TEXT("%s%%"), *UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue * 100.f, 2)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetDefendRateAttribute() || InAttributeChangeData.Attribute == GetDefendScopeRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetDefendRate(FString::Printf(TEXT("%s%%|%s%%"), *UCommonStatics::SanitizeFloat(GetDefendRate() * 100.f, 2), *UCommonStatics::SanitizeFloat(GetDefendScopeRate() * 100.f, 2)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetPhysicsResAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetPhysicsRes(FString::Printf(TEXT("%s(%s%%)"), *UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue, 2), *UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue / (100.f + InAttributeChangeData.NewValue) * 100.f, 2)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetMagicResAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetMagicRes(FString::Printf(TEXT("%s(%s%%)"), *UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue, 2), *UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue / (100.f + InAttributeChangeData.NewValue) * 100.f, 2)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetToughnessRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetToughnessRate(FString::Printf(TEXT("%s%%"), *UCommonStatics::SanitizeFloat(InAttributeChangeData.NewValue * 100.f, 2)));
		}
	}
	Super::OnAttributeChange(InAttributeChangeData);
}

void ADWPlayerCharacter::OnActorAttached(AActor* InActor)
{
	Super::OnActorAttached(InActor);

	PreviewCapture->ShowOnlyActors.Add(InActor);
}

void ADWPlayerCharacter::OnActorDetached(AActor* InActor)
{
	Super::OnActorDetached(InActor);

	PreviewCapture->ShowOnlyActors.Remove(InActor);
}

void ADWPlayerCharacter::OnTargetLockedOn(AActor* InTargetActor)
{
	Looking->TargetLookingOn(InTargetActor);
}

void ADWPlayerCharacter::OnTargetLockedOff(AActor* InTargetActor)
{
	Looking->TargetLookingOff();
}

void ADWPlayerCharacter::OnTargetSetRotation(AActor* InTargetActor, FRotator InControlRotation)
{
	UCameraModuleStatics::SetCameraRotation(InControlRotation.Yaw, InControlRotation.Pitch);
}

FString ADWPlayerCharacter::GetHeadInfo() const
{
	return FString::Printf(TEXT("Lv.%d \"%s\" (Exp: %d/%d)"), Level, *Name.ToString(), (int32)GetExp(), (int32)GetMaxExp());
}

void ADWPlayerCharacter::SetActorVisible_Implementation(bool bInVisible)
{
	Super::SetActorVisible_Implementation(bInVisible);
}

void ADWPlayerCharacter::SetControlMode_Implementation(EDWCharacterControlMode InControlMode)
{
	Super::SetControlMode_Implementation(InControlMode);
}

void ADWPlayerCharacter::SetGenerateVoxelID(const FPrimaryAssetId& InGenerateVoxelID)
{
	Super::SetGenerateVoxelID(InGenerateVoxelID);
}

void ADWPlayerCharacter::SetNameA(FName InName)
{
	Super::SetNameA(InName);
	
	if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWPlayerCharacter::SetRaceID(FName InRaceID)
{
	Super::SetRaceID(InRaceID);
	
	if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
}

bool ADWPlayerCharacter::SetLevelA(int32 InLevel)
{
	if(!Super::SetLevelA(InLevel)) return false;

	if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
	return true;
}

void ADWPlayerCharacter::SetTeamID(FName InTeamID)
{
	Super::SetTeamID(InTeamID);
	
	if(UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetHeadBox>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
}
