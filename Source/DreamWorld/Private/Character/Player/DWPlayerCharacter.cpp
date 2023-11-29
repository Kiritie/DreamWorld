// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Character/Player/DWPlayerCharacter.h"

#include "Achievement/AchievementModuleStatics.h"
#include "Ability/Character/DWCharacterAttributeSet.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "Asset/AssetModuleStatics.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"
#include "Voxel/Datas/VoxelData.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/AbilityModuleTypes.h"
#include "Ability/Character/AbilityCharacterInventoryBase.h"
#include "Camera/CameraModuleStatics.h"
#include "Character/Player/States/DWPlayerCharacterState_Attack.h"
#include "Character/Player/States/DWPlayerCharacterState_Death.h"
#include "Character/Player/States/DWPlayerCharacterState_Default.h"
#include "Character/Player/States/DWPlayerCharacterState_Dodge.h"
#include "Character/Player/States/DWPlayerCharacterState_Interrupt.h"
#include "Character/States/DWCharacterState_Climb.h"
#include "Character/States/DWCharacterState_Crouch.h"
#include "Character/States/DWCharacterState_Defend.h"
#include "Character/States/DWCharacterState_Fall.h"
#include "Character/States/DWCharacterState_Float.h"
#include "Character/States/DWCharacterState_Fly.h"
#include "Character/States/DWCharacterState_Jump.h"
#include "Character/States/DWCharacterState_Ride.h"
#include "Character/States/DWCharacterState_Static.h"
#include "Character/States/DWCharacterState_Swim.h"
#include "Character/States/DWCharacterState_Walk.h"
#include "FSM/Components/FSMComponent.h"
#include "Common/CommonStatics.h"
#include "Widget/World/WorldWidgetComponent.h"
#include "Widget/WidgetContextBox.h"
#include "Ability/Item/Raw/AbilityRawDataBase.h"
#include "Character/DWCharacterData.h"
#include "Common/Targeting/TargetingComponent.h"
#include "Item/Equip/DWEquipData.h"
#include "Vitality/DWVitality.h"
#include "Voxel/Voxels/VoxelInteract.h"
#include "Voxel/Voxels/Auxiliary/VoxelAuxiliary.h"
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"
#include "Widget/Generate/WidgetGeneratePanel.h"
#include "Widget/Inventory/WidgetInventoryBox.h"

//////////////////////////////////////////////////////////////////////////
// ADWPlayerCharacter

ADWPlayerCharacter::ADWPlayerCharacter()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(69.f);
	GetCapsuleComponent()->SetCapsuleRadius(24.f);

	CharacterHP->SetAutoCreate(false);

	Targeting = CreateDefaultSubobject<UTargetingComponent>(FName("Targeting"));
	Targeting->bShouldControlRotation = false;
	Targeting->TargetableActors = ADWCharacter::StaticClass();
	Targeting->TargetableCollisionChannel = ECC_GameTraceChannel1;
	Targeting->LockedOnWidgetParentSocket = FName("LockPoint");
	Targeting->OnTargetLockedOn.AddDynamic(this, &ADWPlayerCharacter::OnTargetLockedOn);
	Targeting->OnTargetLockedOff.AddDynamic(this, &ADWPlayerCharacter::OnTargetLockedOff);
	Targeting->OnTargetSetRotation.AddDynamic(this, &ADWPlayerCharacter::OnTargetSetRotation);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -70));

	PreviewCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("SceneCapture"));
	PreviewCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	PreviewCapture->OrthoWidth = 100;
	PreviewCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	PreviewCapture->SetupAttachment(RootComponent);
	PreviewCapture->SetRelativeLocationAndRotation(FVector(100, 0, 0), FRotator(0, 180, 0));
	
	MiniMapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("MiniMapCapture"));
	MiniMapCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	MiniMapCapture->OrthoWidth = 1000;
	//MiniMapCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	MiniMapCapture->SetupAttachment(RootComponent);
	MiniMapCapture->SetRelativeLocationAndRotation(FVector(0, 0, 500), FRotator(0, 90, 0));

	FSM->bShowDebugMessage = true;
	FSM->DefaultState = UDWCharacterState_Default::StaticClass();
	FSM->States.Empty();
	FSM->States.Add(UDWPlayerCharacterState_Attack::StaticClass());
	FSM->States.Add(UDWCharacterState_Climb::StaticClass());
	FSM->States.Add(UDWCharacterState_Crouch::StaticClass());
	FSM->States.Add(UDWPlayerCharacterState_Death::StaticClass());
	FSM->States.Add(UDWPlayerCharacterState_Default::StaticClass());
	FSM->States.Add(UDWCharacterState_Defend::StaticClass());
	FSM->States.Add(UDWPlayerCharacterState_Dodge::StaticClass());
	FSM->States.Add(UDWCharacterState_Fall::StaticClass());
	FSM->States.Add(UDWCharacterState_Float::StaticClass());
	FSM->States.Add(UDWCharacterState_Fly::StaticClass());
	FSM->States.Add(UDWPlayerCharacterState_Interrupt::StaticClass());
	FSM->States.Add(UDWCharacterState_Jump::StaticClass());
	FSM->States.Add(UDWCharacterState_Ride::StaticClass());
	FSM->States.Add(UDWCharacterState_Static::StaticClass());
	FSM->States.Add(UDWCharacterState_Swim::StaticClass());
	FSM->States.Add(UDWCharacterState_Walk::StaticClass());

	AutoPossessAI = EAutoPossessAI::Disabled;
}

void ADWPlayerCharacter::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();

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
			SaveData.InventoryData = SaveData.GetItemData<UDWCharacterData>().InventoryData;

			switch (SaveData.InventoryInitType)
			{
				case EDWInventoryInitType::Empty:
				{
					SaveData.InventoryData.ClearAllItem();
					break;
				}
				case EDWInventoryInitType::All:
				{
					auto VoxelDatas = UAssetModuleStatics::LoadPrimaryAssets<UVoxelData>(FName("Voxel"));
					for (int32 i = 0; i < VoxelDatas.Num(); i++)
					{
						if(VoxelDatas[i]->VoxelType != EVoxelType::Empty && VoxelDatas[i]->VoxelType != EVoxelType::Unknown && VoxelDatas[i]->bMainPart)
						{
							FAbilityItem tmpItem = FAbilityItem(VoxelDatas[i]->GetPrimaryAssetId(), VoxelDatas[i]->MaxCount);
							SaveData.InventoryData.AddItem(tmpItem, { ESlotSplitType::Default, ESlotSplitType::Shortcut });
						}
					}

					auto RawDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilityRawDataBase>(FName("Raw"));
					for (int32 i = 0; i < RawDatas.Num(); i++)
					{
						FAbilityItem tmpItem = FAbilityItem(RawDatas[i]->GetPrimaryAssetId(), RawDatas[i]->MaxCount);
						SaveData.InventoryData.AddItem(tmpItem, { ESlotSplitType::Default, ESlotSplitType::Shortcut });
					}

					auto EquipDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilityEquipDataBase>(FName("Equip"));
					for (int32 i = 0; i < EquipDatas.Num(); i++)
					{
						FAbilityItem tmpItem = FAbilityItem(EquipDatas[i]->GetPrimaryAssetId(), EquipDatas[i]->MaxCount);
						SaveData.InventoryData.AddItem(tmpItem, { ESlotSplitType::Default, ESlotSplitType::Shortcut });
					}
	
					auto PropDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilityPropDataBase>(FName("Prop"));
					for (int32 i = 0; i < PropDatas.Num(); i++)
					{
						FAbilityItem tmpItem = FAbilityItem(PropDatas[i]->GetPrimaryAssetId(), PropDatas[i]->MaxCount);
						SaveData.InventoryData.AddItem(tmpItem, { ESlotSplitType::Default, ESlotSplitType::Shortcut });
					}

					auto SkillDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilitySkillDataBase>(FName("Skill"));
					for (int32 i = 0; i < SkillDatas.Num(); i++)
					{
						FAbilityItem tmpItem = FAbilityItem(SkillDatas[i]->GetPrimaryAssetId(), SkillDatas[i]->MaxCount);
						SaveData.InventoryData.AddItem(tmpItem, { ESlotSplitType::Default, ESlotSplitType::Shortcut });
					}
					break;
				}
				default: break;
			}
		}
	}

	if(PHASEC(InPhase, EPhase::All))
	{
		SetBodyColor(SaveData.BodyColorIndex);
		SetCapeColor(SaveData.CapeColorIndex);
	}

	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWPlayerCharacter::ToData()
{
	static FDWPlayerSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FDWCharacterSaveData>();
	
	SaveData.BodyColorIndex = GetBodyColor();
	SaveData.CapeColorIndex = GetCapeColor();
	return &SaveData;
}

void ADWPlayerCharacter::Death(IAbilityVitalityInterface* InKiller /* = nullptr */)
{
	Super::Death(InKiller);
	if(InKiller)
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("你被 %s 杀死了！"), *InKiller->GetNameV().ToString()));
	}
}

void ADWPlayerCharacter::Kill(IAbilityVitalityInterface* InTarget)
{
	Super::Kill(InTarget);

	if(Cast<ADWCharacter>(InTarget))
	{
		UAchievementModuleStatics::UnlockAchievement(FName("FirstKillMonster"));
	}
	else if(Cast<ADWVitality>(InTarget))
	{
		UAchievementModuleStatics::UnlockAchievement(FName("FirstKillVitality"));
	}
}

bool ADWPlayerCharacter::CanLookAtTarget(ADWCharacter* InTargetCharacter)
{
	return true;
}

void ADWPlayerCharacter::DoLookAtTarget(ADWCharacter* InTargetCharacter)
{
	if(IsAttacking() || IsDefending())
	{
		const FVector tmpDirection = InTargetCharacter->GetActorLocation() - GetActorLocation();
		SetActorRotation(FRotator(0, tmpDirection.ToOrientationRotator().Yaw, 0));
	}
}

void ADWPlayerCharacter::ChangeHand()
{
	TArray<UAbilityInventorySlot*> AuxiliarySlots = Inventory->GetSlotsBySplitType(ESlotSplitType::Auxiliary);
	if(AuxiliarySlots.Num() > 0 && Inventory->GetSelectedSlot())
	{
		AuxiliarySlots[0]->Replace(Inventory->GetSelectedSlot());
	}
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

	switch (ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>())
			{
				UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SetSkillBoxVisible(true);
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>())
			{
				UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SetSkillBoxVisible(false);
			}
			break;
		}
	}
}

void ADWPlayerCharacter::SetGenerateVoxelID(const FPrimaryAssetId& InGenerateVoxelID)
{
	Super::SetGenerateVoxelID(InGenerateVoxelID);
}

void ADWPlayerCharacter::OnTargetLockedOn(AActor* InTargetActor)
{
	SetLockedTarget(Cast<ADWCharacter>(InTargetActor));
}

void ADWPlayerCharacter::OnTargetLockedOff(AActor* InTargetActor)
{
	SetLockedTarget(nullptr);
}

void ADWPlayerCharacter::OnTargetSetRotation(AActor* InTargetActor, FRotator InControlRotation)
{
	UCameraModuleStatics::SetCameraRotation(InControlRotation.Yaw, InControlRotation.Pitch);
}

bool ADWPlayerCharacter::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	switch (InInteractAction)
	{
		case EInteractAction::Revive:
		{
			if(InInteractionAgent == this)
			{
				return IsDead(false);
			}
		}
		default: break;
	}
	return Super::CanInteract(InInteractAction, InInteractionAgent);
}

void ADWPlayerCharacter::OnEnterInteract(IInteractionAgentInterface* InInteractionAgent)
{
	Super::OnEnterInteract(InInteractionAgent);

	if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->ShowInteractActions(GetInteractableActions());
	}
}

void ADWPlayerCharacter::OnLeaveInteract(IInteractionAgentInterface* InInteractionAgent)
{
	Super::OnLeaveInteract(InInteractionAgent);

	if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->HideInteractActions();
	}
}

void ADWPlayerCharacter::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassivity)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassivity);

	if(bPassivity) return;

	switch(InInteractAction)
	{
		case EVoxelInteractAction::Open:
		{
			if(AVoxelInteractAuxiliary* InteractionAgent = Cast<AVoxelInteractAuxiliary>(InInteractionAgent))
			{
				switch(InteractionAgent->GetVoxelItem().GetVoxelType())
				{
					case EVoxelType::Chest:
					{
						UWidgetModuleStatics::OpenUserWidget<UWidgetInventoryBox>({ InteractionAgent });
						FDelegateHandle DelegateHandle = UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBox>()->OnClosed.AddLambda([this, InteractionAgent, DelegateHandle](bool bInstant)
						{
							DoInteract((EInteractAction)EVoxelInteractAction::Close, InteractionAgent);
							UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBox>()->OnClosed.Remove(DelegateHandle);
						});
						break;
					}
					case EVoxelType::Furnace:
					case EVoxelType::Crafting_Table:
					{
						UWidgetModuleStatics::OpenUserWidget<UWidgetGeneratePanel>({ InteractionAgent });
						FDelegateHandle DelegateHandle = UWidgetModuleStatics::GetUserWidget<UWidgetGeneratePanel>()->OnClosed.AddLambda([this, InteractionAgent, DelegateHandle](bool bInstant)
						{
							DoInteract((EInteractAction)EVoxelInteractAction::Close, InteractionAgent);
							UWidgetModuleStatics::GetUserWidget<UWidgetGeneratePanel>()->OnClosed.Remove(DelegateHandle);
						});
						break;
					}
					default: break;
				}
			}
			break;
		}
		case EVoxelInteractAction::Close:
		{
			if(!IsOverlapping(InInteractionAgent))
			{
				SetInteractingAgent(nullptr, true);
			}
			break;
		}
		default: break;
	}
	if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->ShowInteractActions(GetInteractableActions());
	}
}

bool ADWPlayerCharacter::OnInteractVoxel(const FVoxelHitResult& InVoxelHitResult, EInputInteractAction InInteractAction)
{
	return Super::OnInteractVoxel(InVoxelHitResult, InInteractAction);
}

void ADWPlayerCharacter::Turn_Implementation(float InValue)
{
	if(IsBreakAllInput()) return;

	Targeting->TargetActorWithAxisInput(InValue);
}

void ADWPlayerCharacter::MoveForward_Implementation(float InValue)
{
	if(IsBreakAllInput()) return;

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
	if(IsBreakAllInput()) return;
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, InValue);
}

void ADWPlayerCharacter::MoveUp_Implementation(float InValue)
{
	if(IsBreakAllInput()) return;
	
	if(IsFlying() || IsSwimming() || IsFloating())
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(FVector(Direction.X * 0.2f, Direction.Y * 0.2f, 0.5f * InValue), 1.f);
	}
}

void ADWPlayerCharacter::OnAssembleItem(const FAbilityItem& InItem)
{
	Super::OnAssembleItem(InItem);

	const auto& ItemData = InItem.GetData<UAbilityItemDataBase>();
	switch(ItemData.GetItemType())
	{
		case EAbilityItemType::Equip:
		{
			const auto& EquipData = InItem.GetData<UDWEquipData>();
			if(AAbilityEquipBase* Equip = GetEquip(EquipData.PartType))
			{
				PreviewCapture->ShowOnlyActors.Add(Equip);
			}
			break;
		}
		default: break;
	}
}

void ADWPlayerCharacter::OnDischargeItem(const FAbilityItem& InItem)
{
	Super::OnDischargeItem(InItem);

	const auto& ItemData = InItem.GetData<UAbilityItemDataBase>();
	switch(ItemData.GetItemType())
	{
		case EAbilityItemType::Equip:
		{
			const auto& EquipData = InItem.GetData<UDWEquipData>();
			if(AAbilityEquipBase* Equip = GetEquip(EquipData.PartType))
			{
				PreviewCapture->ShowOnlyActors.Remove(Equip);
			}
			break;
		}
		default: break;
	}
}

void ADWPlayerCharacter::OnSelectItem(const FAbilityItem& InItem)
{
	Super::OnSelectItem(InItem);
}

void ADWPlayerCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;
	
	if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetHealthAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxHealthAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->SetHealthPercent(GetHealth(), GetMaxHealth());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHealthInfo(FString::Printf(TEXT("%d/%d"), (int32)GetHealth(), (int32)GetMaxHealth()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetExpAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>())
		{
			if(DeltaValue > 0.f)
			{
				UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("获得: %d点经验值"), (int32)DeltaValue));
			}
			if(InAttributeChangeData.NewValue >= GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxExp())
			{
				UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("你已升到 %d 级！"), Level));
			}
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxExpAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetManaAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxManaAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->SetManaPercent(GetMana(), GetMaxMana());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetManaInfo(FString::Printf(TEXT("%d/%d"), (int32)GetMana(), (int32)GetMaxMana()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxStaminaAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetStaminaInfo(FString::Printf(TEXT("%d/%d"), (int32)GetStamina(), (int32)GetMaxStamina()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMoveSpeedAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetMoveSpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetSwimSpeedAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetSwimSpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetRideSpeedAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetRideSpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetFlySpeedAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetFlySpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetRotationSpeedAttribute())
	{
		GetCharacterMovement()->RotationRate = FRotator(0, InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1), 0) * RotationRate;
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackForceAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetAttackForce(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetRepulseForceAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetRepulseForce(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackSpeedAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetAttackSpeed(FString::SanitizeFloat(InAttributeChangeData.NewValue, 1));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackCritRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetAttackCritRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackStealRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetAttackStealRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetDefendRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetDefendRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetPhysicsDefRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetPhysicsDefRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMagicDefRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetMagicDefRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetToughnessRateAttribute())
	{
		if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetToughnessRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	Super::OnAttributeChange(InAttributeChangeData);
}

void ADWPlayerCharacter::SetNameV(FName InName)
{
	Super::SetNameV(InName);
	if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWPlayerCharacter::SetRaceID(FName InRaceID)
{
	Super::SetRaceID(InRaceID);
	if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
}

bool ADWPlayerCharacter::SetLevelV(int32 InLevel)
{
	if(!Super::SetLevelV(InLevel)) return false;

	if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
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
	if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
}
