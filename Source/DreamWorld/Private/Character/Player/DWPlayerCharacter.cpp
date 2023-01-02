// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Character/Player/DWPlayerCharacter.h"

#include "AchievementSubSystem.h"
#include "Ability/Character/DWCharacterAttributeSet.h"
#include "Ability/Components/InteractionComponent.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Gameplay/DWPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Ability/Inventory/Slot/InventorySlot.h"
#include "Gameplay/DWGameState.h"
#include "Widget/WidgetGameHUD.h"
#include "Voxel/DWVoxelModule.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Voxel/Datas/VoxelData.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/AbilityModuleTypes.h"
#include "Camera/CameraModuleBPLibrary.h"
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
#include "Global/GlobalBPLibrary.h"
#include "Voxel/Voxels/Entity/VoxelEntity.h"
#include "Widget/World/WorldWidgetComponent.h"
#include "Ability/Inventory/CharacterInventory.h"
#include "Widget/WidgetContextBox.h"
#include "Ability/Item/Raw/AbilityRawDataBase.h"
#include "Gameplay/WHGameInstance.h"
#include "Vitality/DWVitality.h"

//////////////////////////////////////////////////////////////////////////
// ADWPlayerCharacter

ADWPlayerCharacter::ADWPlayerCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance ifyou don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(69.f);
	GetCapsuleComponent()->SetCapsuleRadius(24.f);

	CharacterHP->SetAutoCreate(false);

	TargetSystem = CreateDefaultSubobject<UTargetSystemComponent>(FName("TargetSystem"));
	TargetSystem->bShouldControlRotation = false;
	TargetSystem->TargetableActors = ADWCharacter::StaticClass();
	TargetSystem->TargetableCollisionChannel = ECC_GameTraceChannel1;
	TargetSystem->LockedOnWidgetParentSocket = FName("LockPoint");
	TargetSystem->OnTargetLockedOn.AddDynamic(this, &ADWPlayerCharacter::OnTargetLockedOn);
	TargetSystem->OnTargetLockedOff.AddDynamic(this, &ADWPlayerCharacter::OnTargetLockedOff);
	TargetSystem->OnTargetSetRotation.AddDynamic(this, &ADWPlayerCharacter::OnTargetSetRotation);
	
	// Create a camera boom (pulls in towards the player ifthere is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200; // The camera follows at this distance behind the character	
	CameraBoom->SocketOffset = FVector(0, 30, 40);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

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

void ADWPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
}

// Called when the game starts or when spawned
void ADWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PreviewCapture->ShowOnlyActors.Add(this);
}

void ADWPlayerCharacter::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void ADWPlayerCharacter::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();
}

void ADWPlayerCharacter::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWPlayerSaveData>();
	
	switch(InPhase)
	{
		case EPhase::Primary:
		case EPhase::Second:
		case EPhase::Final:
		{
			SetBodyColor(SaveData.BodyColorIndex);
			SetCapeColor(SaveData.CapeColorIndex);

			if(!SaveData.IsSaved())
			{
				switch (SaveData.InventoryInitType)
				{
					case EDWPlayerInventoryInitType::Default:
					{
						break;
					}
					case EDWPlayerInventoryInitType::Empty:
					{
						SaveData.InventoryData.ClearAllItem();
						break;
					}
					case EDWPlayerInventoryInitType::All:
					{
						SaveData.InventoryData.ClearAllItem();

						auto VoxelDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UVoxelData>(UAbilityModuleBPLibrary::ItemTypeToAssetType(EAbilityItemType::Voxel));
						for (int32 i = 0; i < VoxelDatas.Num(); i++)
						{
							if(VoxelDatas[i]->VoxelType != EVoxelType::Empty && VoxelDatas[i]->VoxelType != EVoxelType::Unknown && VoxelDatas[i]->PartType == EVoxelPartType::Main)
							{
								FAbilityItem tmpItem = FAbilityItem(VoxelDatas[i]->GetPrimaryAssetId(), VoxelDatas[i]->MaxCount);
								SaveData.InventoryData.AddItem(tmpItem);
							}
						}
		
						auto RawDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilityRawDataBase>(UAbilityModuleBPLibrary::ItemTypeToAssetType(EAbilityItemType::Raw));
						for (int32 i = 0; i < RawDatas.Num(); i++)
						{
							FAbilityItem tmpItem = FAbilityItem(RawDatas[i]->GetPrimaryAssetId(), RawDatas[i]->MaxCount);
							SaveData.InventoryData.AddItem(tmpItem);
						}
		
						auto EquipDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilityEquipDataBase>(UAbilityModuleBPLibrary::ItemTypeToAssetType(EAbilityItemType::Equip));
						for (int32 i = 0; i < EquipDatas.Num(); i++)
						{
							FAbilityItem tmpItem = FAbilityItem(EquipDatas[i]->GetPrimaryAssetId(), EquipDatas[i]->MaxCount);
							SaveData.InventoryData.AddItem(tmpItem);
						}
			
						auto PropDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilityPropDataBase>(UAbilityModuleBPLibrary::ItemTypeToAssetType(EAbilityItemType::Prop));
						for (int32 i = 0; i < PropDatas.Num(); i++)
						{
							FAbilityItem tmpItem = FAbilityItem(PropDatas[i]->GetPrimaryAssetId(), PropDatas[i]->MaxCount);
							SaveData.InventoryData.AddItem(tmpItem);
						}

						auto SkillDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilitySkillDataBase>(UAbilityModuleBPLibrary::ItemTypeToAssetType(EAbilityItemType::Skill));
						for (int32 i = 0; i < SkillDatas.Num(); i++)
						{
							FAbilityItem tmpItem = FAbilityItem(SkillDatas[i]->GetPrimaryAssetId(), SkillDatas[i]->MaxCount);
							SaveData.InventoryData.AddItem(tmpItem);
						}
						break;
					}
				}
			}
			break;
		}
	}

	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWPlayerCharacter::ToData()
{
	static FDWPlayerSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FDWCharacterSaveData>();
	SaveData.CameraRotation = UCameraModuleBPLibrary::GetCurrentCameraRotation();
	SaveData.BodyColorIndex = GetBodyColor();
	SaveData.CapeColorIndex = GetCapeColor();
	return &SaveData;
}

// Called every frame
void ADWPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead()) return;
}

void ADWPlayerCharacter::Death(IAbilityVitalityInterface* InKiller /* = nullptr */)
{
	Super::Death(InKiller);
	if(InKiller)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("你被 %s 杀死了！"), *InKiller->GetNameV().ToString()));
	}
}

void ADWPlayerCharacter::Kill(IAbilityVitalityInterface* InTarget)
{
	Super::Kill(InTarget);

	if(Cast<ADWCharacter>(InTarget))
	{
		UGlobalBPLibrary::GetGameInstance()->GetSubsystem<UAchievementSubSystem>()->Unlock(FName("FirstKillMonster"));
	}
	else if(Cast<ADWVitality>(InTarget))
	{
		UGlobalBPLibrary::GetGameInstance()->GetSubsystem<UAchievementSubSystem>()->Unlock(FName("FirstKillVitality"));
	}
}

bool ADWPlayerCharacter::CanLookAtTarget(ADWCharacter* InTargetCharacter)
{
	return true;
}

void ADWPlayerCharacter::LookAtTarget(ADWCharacter* InTargetCharacter)
{
	if(IsAttacking() || IsDefending())
	{
		const FVector tmpDirection = InTargetCharacter->GetActorLocation() - GetActorLocation();
		SetActorRotation(FRotator(0, tmpDirection.ToOrientationRotator().Yaw, 0));
	}
}

FString ADWPlayerCharacter::GetHeadInfo() const
{
	return FString::Printf(TEXT("Lv.%d \"%s\" (Exp: %d/%d)"), Level, *Name.ToString(), (int32)GetExp(), (int32)GetMaxExp());
}

void ADWPlayerCharacter::SetActorVisible_Implementation(bool bNewVisible)
{
	Super::SetActorVisible_Implementation(bNewVisible);

	if(bNewVisible && ControlMode == EDWCharacterControlMode::Fighting)
	{
		if(GenerateVoxelEntity) GenerateVoxelEntity->Execute_SetActorVisible(GenerateVoxelEntity, false);
		HammerMesh->SetVisibility(false);
	}
}

void ADWPlayerCharacter::SetControlMode(EDWCharacterControlMode InControlMode)
{
	Super::SetControlMode(InControlMode);

	switch (ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>())
			{
				UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SetSkillBoxVisible(true);
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>())
			{
				UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SetSkillBoxVisible(false);
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
	UCameraModuleBPLibrary::SetCameraRotation(InControlRotation.Yaw, InControlRotation.Pitch);
}

void ADWPlayerCharacter::RefreshEquip(EDWEquipPartType InPartType, const FAbilityItem& InItem)
{
	if(InItem.IsValid())
	{
		Super::RefreshEquip(InPartType, InItem);
		if(GetEquip(InPartType))
		{
			PreviewCapture->ShowOnlyActors.Add(GetEquip(InPartType));
		}
	}
	else
	{
		if(GetEquip(InPartType))
		{
			PreviewCapture->ShowOnlyActors.Remove(GetEquip(InPartType));
		}
		Super::RefreshEquip(InPartType, InItem);
	}
}

void ADWPlayerCharacter::OnEnterInteract(IInteractionAgentInterface* InInteractionAgent)
{
	Super::OnEnterInteract(InInteractionAgent);
}

void ADWPlayerCharacter::OnLeaveInteract(IInteractionAgentInterface* InInteractionAgent)
{
	Super::OnLeaveInteract(InInteractionAgent);
}

void ADWPlayerCharacter::OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction)
{
	Super::OnInteract(InInteractionAgent, InInteractAction);
}

void ADWPlayerCharacter::ChangeHand()
{
	TArray<UInventorySlot*> AuxilarySlots = Inventory->GetSplitSlots(ESplitSlotType::Auxiliary);
	if(AuxilarySlots.Num() > 0 && Inventory->GetSelectedSlot())
	{
		AuxilarySlots[0]->Replace(Inventory->GetSelectedSlot());
	}
}

void ADWPlayerCharacter::Turn_Implementation(float InValue)
{
	if(IsBreakAllInput()) return;

	TargetSystem->TargetActorWithAxisInput(InValue);
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

void ADWPlayerCharacter::OnSelectItem(const FAbilityItem& InItem)
{
	Super::OnSelectItem(InItem);
}

void ADWPlayerCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;
	
	if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetHealthAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxHealthAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHealthPercent(GetHealth(), GetMaxHealth());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHealthInfo(FString::Printf(TEXT("%d/%d"), (int32)GetHealth(), (int32)GetMaxHealth()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetExpAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetContextBox>())
		{
			if(DeltaValue > 0.f)
			{
				UWidgetModuleBPLibrary::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("获得: %d点经验值"), (int32)DeltaValue));
			}
			if(InAttributeChangeData.NewValue >= GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxExp())
			{
				UWidgetModuleBPLibrary::GetUserWidget<UWidgetContextBox>()->AddMessage(FString::Printf(TEXT("你已升到 %d 级！"), Level));
			}
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxExpAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetManaAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxManaAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetManaPercent(GetMana(), GetMaxMana());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetManaInfo(FString::Printf(TEXT("%d/%d"), (int32)GetMana(), (int32)GetMaxMana()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetStaminaAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxStaminaAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetStaminaPercent(GetStamina(), GetMaxStamina());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetStaminaInfo(FString::Printf(TEXT("%d/%d"), (int32)GetStamina(), (int32)GetMaxStamina()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMoveSpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetMoveSpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetSwimSpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetSwimSpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetRideSpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetRideSpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetFlySpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetFlySpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetRotationSpeedAttribute())
	{
		GetCharacterMovement()->RotationRate = FRotator(0, InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1), 0) * RotationRate;
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackForceAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetAttackForce(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetRepulseForceAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetRepulseForce(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackSpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetAttackSpeed(FString::SanitizeFloat(InAttributeChangeData.NewValue, 1));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackCritRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetAttackCritRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetAttackStealRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetAttackStealRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetDefendRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetDefendRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetPhysicsDefRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetPhysicsDefRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMagicDefRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetMagicDefRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetToughnessRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetToughnessRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	Super::OnAttributeChange(InAttributeChangeData);
}

void ADWPlayerCharacter::SetNameV(FName InName)
{
	Super::SetNameV(InName);
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWPlayerCharacter::SetRaceID(FName InRaceID)
{
	Super::SetRaceID(InRaceID);
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
}

bool ADWPlayerCharacter::SetLevelV(int32 InLevel)
{
	if(!Super::SetLevelV(InLevel)) return false;

	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
	return true;
}

void ADWPlayerCharacter::SetTeamID(FName InTeamID)
{
	Super::SetTeamID(InTeamID);
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
}
