// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Character/Player/DWPlayerCharacter.h"

#include "Ability/Character/DWCharacterAttributeSet.h"
#include "Ability/Components/InteractionComponent.h"
#include "Item/Equip/Shield/DWEquipShield.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Ability/Item/AbilityItemDataBase.h"
#include "Character/Player/DWPlayerCharacterAnim.h"
#include "Gameplay/DWPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Gameplay/DWGameMode.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/Components/WidgetCharacterHPComponent.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Inventory/CharacterInventory.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Gameplay/DWGameState.h"
#include "Inventory/Slot/InventorySkillSlot.h"
#include "Gameplay/DWGameInstance.h"
#include "Input/InputModuleBPLibrary.h"
#include "Widget/WidgetGameHUD.h"
#include "Inventory/Slot/InventoryEquipSlot.h"
#include "Voxel/DWVoxelModule.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Voxels/Voxel.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Voxel/Datas/VoxelData.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/AbilityModuleTypes.h"
#include "Ability/Components/CharacterInteractionComponent.h"
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
#include "Voxel/VoxelModuleBPLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ADWPlayerCharacter

ADWPlayerCharacter::ADWPlayerCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance ifyou don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(69);
	GetCapsuleComponent()->SetCapsuleRadius(24);
	GetCapsuleComponent()->SetCollisionProfileName(FName("Character"));

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->AirControl = 0.3f;

	CharacterHP->SetAutoCreate(false);

	TargetSystem = CreateDefaultSubobject<UTargetSystemComponent>(FName("TargetSystem"));
	TargetSystem->bShouldControlRotation = true;
	TargetSystem->TargetableActors = ADWCharacter::StaticClass();
	TargetSystem->TargetableCollisionChannel = ECC_GameTraceChannel1;
	TargetSystem->LockedOnWidgetParentSocket = FName("LockPoint");
	
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

	VoxelMesh = CreateDefaultSubobject<UVoxelMeshComponent>(FName("VoxelMesh"));
	VoxelMesh->SetupAttachment(GetMesh(), FName("VoxelMesh"));
	VoxelMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	VoxelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VoxelMesh->SetCastShadow(false);
	VoxelMesh->SetVisibility(false);
	VoxelMesh->Initialize(EVoxelMeshType::PickUp);

	HammerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("HammerMesh"));
	HammerMesh->SetupAttachment(GetMesh(), FName("HammerMesh"));
	HammerMesh->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0, 0));
	HammerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HammerMesh->SetCastShadow(false);
	HammerMesh->SetVisibility(false);

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

	// states
	ControlMode = EDWCharacterControlMode::Fighting;
	Nature = EDWCharacterNature::Player;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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

void ADWPlayerCharacter::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	auto& SaveData = InSaveData->CastRef<FDWPlayerSaveData>();
	
	if(!bForceMode && !SaveData.IsSaved())
	{
		switch (SaveData.InventoryInitType)
		{
			case EDWPlayerInventoryInitType::Small:
			{
				break;
			}
			case EDWPlayerInventoryInitType::Fill:
			{
				auto VoxelDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UVoxelData>(UAbilityModuleBPLibrary::ItemTypeToAssetType(EAbilityItemType::Voxel));
				for (int32 i = 0; i < VoxelDatas.Num(); i++)
				{
					if(VoxelDatas[i]->VoxelType != EVoxelType::Empty && VoxelDatas[i]->VoxelType != EVoxelType::Unknown)
					{
						FAbilityItem tmpItem = FAbilityItem(VoxelDatas[i]->GetPrimaryAssetId(), VoxelDatas[i]->MaxCount);
						SaveData.InventoryData.AddItem(tmpItem);
					}
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
			default: break;
		}
	}

	Super::LoadData(InSaveData, bForceMode);
}

FSaveData* ADWPlayerCharacter::ToData()
{
	static FDWPlayerSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FDWCharacterSaveData>();
	SaveData.ControlMode = ControlMode;
	return &SaveData;
}

// Called every frame
void ADWPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead()) return;
}

void ADWPlayerCharacter::LookAtTarget(ADWCharacter* InTargetCharacter)
{
	Super::LookAtTarget(InTargetCharacter);
	const FVector tmpDirection = InTargetCharacter->GetActorLocation() + FVector::DownVector * InTargetCharacter->GetHalfHeight() - GetActorLocation();
	const FRotator tmpRotator = tmpDirection.ToOrientationRotator();
	GetController<ADWPlayerController>()->SetControlRotation(tmpRotator);
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
		VoxelMesh->SetVisibility(false);
		HammerMesh->SetVisibility(false);
	}
}

void ADWPlayerCharacter::SetControlMode(EDWCharacterControlMode InControlMode)
{
	Super::SetControlMode(InControlMode);

	if(!Execute_IsVisible(this)) return;
	
	switch (ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			VoxelMesh->SetVisibility(false);
			HammerMesh->SetVisibility(false);
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			VoxelMesh->SetVisibility(true);
			HammerMesh->SetVisibility(true);
			break;
		}
	}
}

void ADWPlayerCharacter::SetGenerateVoxelItem(FVoxelItem InGenerateVoxelItem)
{
	Super::SetGenerateVoxelItem(InGenerateVoxelItem);

	if(!GenerateVoxelItem.IsValid())
	{
		VoxelMesh->BuildVoxel(GenerateVoxelItem);
		VoxelMesh->CreateMesh(0, false);
	}
	else
	{
		VoxelMesh->ClearMesh();
	}
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

	UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->RefreshActions();
}

void ADWPlayerCharacter::OnLeaveInteract(IInteractionAgentInterface* InInteractionAgent)
{
	Super::OnLeaveInteract(InInteractionAgent);

	UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->RefreshActions();
}

void ADWPlayerCharacter::MoveForward_Implementation(float InValue)
{
	if(IsBreakAllInput()) return;

	FVector Direction;
	const FRotator Rotation = GetControlRotation();
	if(IsFlying() || IsSwimming())
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
	
	if(IsFlying() || IsSwimming())
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(FVector(Direction.X * 0.1f, Direction.Y * 0.1f, 1.f) * InValue, 0.5f);
	}
}

bool ADWPlayerCharacter::UseItem(FAbilityItem& InItem)
{
	if((InItem.GetData().GetItemType() == EAbilityItemType::Voxel))
	{
		if(ControlMode == EDWCharacterControlMode::Creating)
		{
			FVoxelHitResult voxelHitResult;
			if (UGlobalBPLibrary::GetPlayerController<ADWPlayerController>()->RaycastVoxel(voxelHitResult))
			{
				return GenerateVoxel(static_cast<FVoxelItem&>(InItem), voxelHitResult);
			}
		}
	}
	else if((InItem.GetData().GetItemType() == EAbilityItemType::Prop))
	{
		return Super::UseItem(InItem);
	}
	return false;
}

void ADWPlayerCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	Super::OnAttributeChange(InAttributeChangeData);

	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;
	
	if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetHealthAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHealthPercent(InAttributeChangeData.NewValue, GetMaxHealth());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHealthInfo(FString::Printf(TEXT("%d/%d"), (int32)InAttributeChangeData.NewValue, (int32)GetMaxHealth()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetExpAttribute())
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
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxHealthAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHealthPercent(InAttributeChangeData.NewValue, GetMaxHealth());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHealthInfo(FString::Printf(TEXT("%d/%d"), (int32)InAttributeChangeData.NewValue, (int32)GetMaxHealth()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetManaAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetManaPercent(InAttributeChangeData.NewValue, GetMaxMana());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetManaInfo(FString::Printf(TEXT("%d/%d"), (int32)InAttributeChangeData.NewValue, (int32)GetMaxMana()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxManaAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetManaPercent(InAttributeChangeData.NewValue, GetMaxMana());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetManaInfo(FString::Printf(TEXT("%d/%d"), (int32)InAttributeChangeData.NewValue, (int32)GetMaxMana()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxStaminaAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetStaminaPercent(InAttributeChangeData.NewValue, GetMaxStamina());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetStaminaInfo(FString::Printf(TEXT("%d/%d"), (int32)InAttributeChangeData.NewValue, (int32)GetMaxStamina()));
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWCharacterAttributeSet>()->GetMaxStaminaAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetStaminaPercent(InAttributeChangeData.NewValue, GetMaxStamina());
		}
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetStaminaInfo(FString::Printf(TEXT("%d/%d"), (int32)InAttributeChangeData.NewValue, (int32)GetMaxStamina()));
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

void ADWPlayerCharacter::SetLevelV(int32 InLevel)
{
	Super::SetLevelV(InLevel);
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetHeadInfo(GetHeadInfo());
	}
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetHeadInfo(GetHeadInfo());
	}
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
