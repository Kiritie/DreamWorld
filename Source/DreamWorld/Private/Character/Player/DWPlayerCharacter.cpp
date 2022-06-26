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

	// states
	ControlMode = EDWControlMode::Fighting;
	Nature = EDWCharacterNature::Player;

	// stats
	VoxelItem = FVoxelItem::EmptyVoxel;

	// inputs
	bPressedAttack = false;
	bPressedDefend = false;

	AttackAbilityQueue = 0;

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

	PlayerInputComponent->SetTickableWhenPaused(true);

	PlayerInputComponent->BindAction("Interact1", IE_Pressed, this, &ADWPlayerCharacter::DoInteractAction1);
	PlayerInputComponent->BindAction("Interact2", IE_Pressed, this, &ADWPlayerCharacter::DoInteractAction1);
	PlayerInputComponent->BindAction("Interact3", IE_Pressed, this, &ADWPlayerCharacter::DoInteractAction1);
	PlayerInputComponent->BindAction("Interact4", IE_Pressed, this, &ADWPlayerCharacter::DoInteractAction1);
	PlayerInputComponent->BindAction("Interact5", IE_Pressed, this, &ADWPlayerCharacter::DoInteractAction1);

	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ADWPlayerCharacter::OnDodgePressed);
	PlayerInputComponent->BindAction("Dodge", IE_Released, this, &ADWPlayerCharacter::OnDodgeReleased);

	PlayerInputComponent->BindAction("ToggleCrouch", IE_Released, this, &ADWPlayerCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("ToggleControlMode", IE_Pressed, this, &ADWPlayerCharacter::ToggleControlMode);
	PlayerInputComponent->BindAction("ToggleLockSightTarget", IE_Pressed, this, &ADWPlayerCharacter::ToggleLockTarget);

	PlayerInputComponent->BindAction("Attack/Destroy", IE_Pressed, this, &ADWPlayerCharacter::OnAttackDestroyPressed);
	PlayerInputComponent->BindAction("Attack/Destroy", IE_Released, this, &ADWPlayerCharacter::OnAttackDestroyReleased);

	PlayerInputComponent->BindAction("Defend/Generate", IE_Pressed, this, &ADWPlayerCharacter::OnDefendGeneratePressed);
	PlayerInputComponent->BindAction("Defend/Generate", IE_Released, this, &ADWPlayerCharacter::OnDefendGenerateReleased);

	PlayerInputComponent->BindAction("ReleaseSkillAbility1", IE_Pressed, this, &ADWPlayerCharacter::ReleaseSkillAbility1);
	PlayerInputComponent->BindAction("ReleaseSkillAbility2", IE_Pressed, this, &ADWPlayerCharacter::ReleaseSkillAbility2);
	PlayerInputComponent->BindAction("ReleaseSkillAbility3", IE_Pressed, this, &ADWPlayerCharacter::ReleaseSkillAbility3);
	PlayerInputComponent->BindAction("ReleaseSkillAbility4", IE_Pressed, this, &ADWPlayerCharacter::ReleaseSkillAbility4);
}

// Called when the game starts or when spawned
void ADWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	WidgetCharacterHP->SetWidget(nullptr);

	PreviewCapture->ShowOnlyActors.Add(this);

	//AVoxelModule::GetCurrent()->CreateTeam();
}

// Called every frame
void ADWPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead()) return;

	if(IsActive())
	{
		if(bPressedAttack)
		{
			OnAttackDestroyRepeat();
		}

		if(bPressedDefend)
		{
			OnDefendGenerateRepeat();
		}

		switch (ControlMode)
		{
			case EDWControlMode::Fighting:
			{
				if(bPressedAttack || AttackAbilityQueue > 0)
				{
					Attack();
				}
				else if(IsFreeToAnim() && AttackType == EDWAttackType::NormalAttack)
				{
					UnAttack();
				}

				if(bPressedDefend)
				{
					Defend();
				}
				else
				{
					UnDefend();
				}
			}
			case EDWControlMode::Creating:
			{
				// FVoxelHitResult voxelHitResult;
				// if(RaycastVoxel(voxelHitResult))
				// {
				// 	voxelHitResult.GetVoxel()->OnMouseHover(voxelHitResult);
				// 	UVoxel::DespawnVoxel(voxelHitResult.GetVoxel());
				// }
				break;
			}
		}
	}
	else if(GetActorLocation().IsNearlyZero())
	{
		FHitResult hitResult;
		const FVector rayStart = FVector(GetActorLocation().X, GetActorLocation().Y, ADWVoxelModule::GetWorldData()->ChunkHeightRange * ADWVoxelModule::GetWorldData()->GetChunkLength() + 500);
		const FVector rayEnd = FVector(GetActorLocation().X, GetActorLocation().Y, 0);
		if(AMainModule::GetModuleByClass<AVoxelModule>()->ChunkTraceSingle(rayStart, rayEnd, GetRadius(), GetHalfHeight(), hitResult))
		{
			SetActorLocationAndRotation(hitResult.Location, FRotator::ZeroRotator);
			Active();
		}
	}
}

void ADWPlayerCharacter::LoadData(FSaveData* InSaveData)
{
	Super::LoadData(InSaveData);

	auto SaveData = *static_cast<FDWPlayerSaveData*>(InSaveData);
	if (SaveData.bSaved)
	{
		SetControlMode(SaveData.ControlMode);
	}
	else
	{
		SetControlMode(EDWControlMode::Fighting);

		auto VoxelDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UVoxelData>(UAbilityModuleBPLibrary::GetAssetTypeByItemType(EAbilityItemType::Voxel));
		for (int32 i = 0; i < VoxelDatas.Num(); i++)
		{
			if(VoxelDatas[i]->VoxelType != EVoxelType::Empty && VoxelDatas[i]->VoxelType != EVoxelType::Unknown)
			{
				FAbilityItem tmpItem = FAbilityItem(VoxelDatas[i]->GetPrimaryAssetId(), VoxelDatas[i]->MaxCount);
				Inventory->AdditionItemByRange(tmpItem);
			}
		}
		
		auto EquipDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilityEquipDataBase>(UAbilityModuleBPLibrary::GetAssetTypeByItemType(EAbilityItemType::Equip));
		for (int32 i = 0; i < EquipDatas.Num(); i++)
		{
			FAbilityItem tmpItem = FAbilityItem(EquipDatas[i]->GetPrimaryAssetId(), EquipDatas[i]->MaxCount);
			Inventory->AdditionItemByRange(tmpItem);
		}
			
		auto PropDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilityPropDataBase>(UAbilityModuleBPLibrary::GetAssetTypeByItemType(EAbilityItemType::Prop));
		for (int32 i = 0; i < PropDatas.Num(); i++)
		{
			FAbilityItem tmpItem = FAbilityItem(PropDatas[i]->GetPrimaryAssetId(), PropDatas[i]->MaxCount);
			Inventory->AdditionItemByRange(tmpItem);
		}

		auto SkillDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilitySkillDataBase>(UAbilityModuleBPLibrary::GetAssetTypeByItemType(EAbilityItemType::Skill));
		for (int32 i = 0; i < SkillDatas.Num(); i++)
		{
			FAbilityItem tmpItem = FAbilityItem(SkillDatas[i]->GetPrimaryAssetId(), SkillDatas[i]->MaxCount);
			Inventory->AdditionItemByRange(tmpItem);
		}
	}
}

FSaveData* ADWPlayerCharacter::ToData()
{
	static auto SaveData = FDWPlayerSaveData();
	SaveData.ControlMode = ControlMode;
	return &SaveData;
}

void ADWPlayerCharacter::Active(bool bResetData)
{
	Super::Active(bResetData);
}

void ADWPlayerCharacter::Disable(bool bDisableMovement, bool bDisableCollision)
{
	Super::Disable(bDisableMovement, bDisableCollision);
}

void ADWPlayerCharacter::Revive()
{
	Super::Revive();
	SetControlMode(ControlMode);
}

void ADWPlayerCharacter::Death(AActor* InKiller)
{
	Super::Death(InKiller);
	if(TargetSystem->IsLocked())
	{
		TargetSystem->TargetLockOff();
	}
}

void ADWPlayerCharacter::DeathEnd()
{
	Super::DeathEnd();
	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetCrosshairVisible(false);
	}
}

void ADWPlayerCharacter::ResetData()
{
	Super::ResetData();

	ControlMode = EDWControlMode::Fighting;
	bPressedAttack = false;
	bPressedDefend = false;
	LockedTarget = nullptr;
	if(GetController<ADWPlayerController>())
	{
		GetController<ADWPlayerController>()->ResetData();
	}
}

void ADWPlayerCharacter::Interrupt(float InDuration /* = -1 */, bool bInPlayAnim /* = false */)
{
	Super::Interrupt(InDuration, bInPlayAnim);
	AttackAbilityQueue = 0;
}

void ADWPlayerCharacter::Dodge()
{
	if(!IsDodging())
	{
		TargetSystem->SetShouldControlRotation(false);
	}
	Super::Dodge();
}

void ADWPlayerCharacter::UnDodge()
{
	if(IsDodging())
	{
		TargetSystem->SetShouldControlRotation(true);
	}
	Super::UnDodge();
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
	return FString::Printf(TEXT("Lv.%d \"%s\" (Exp: %d/%d)"), Level, *Name.ToString(), EXP, GetMaxEXP());
}

void ADWPlayerCharacter::SetControlMode(EDWControlMode InControlMode)
{
	switch (ControlMode = InControlMode)
	{
		case EDWControlMode::Fighting:
		{
			VoxelMesh->SetVisibility(false);
			HammerMesh->SetVisibility(false);
			if(GetWeapon()) GetWeapon()->SetVisible(true);
			if(GetShield()) GetShield()->SetVisible(true);
			break;
		}
		case EDWControlMode::Creating:
		{
			VoxelMesh->SetVisibility(true);
			HammerMesh->SetVisibility(true);
			if(GetWeapon()) GetWeapon()->SetVisible(false);
			if(GetShield()) GetShield()->SetVisible(false);
			break;
		}
	}
}

void ADWPlayerCharacter::UnAttack()
{
	Super::UnAttack();
	AttackAbilityQueue = 0;
}

void ADWPlayerCharacter::AttackStart()
{
	Super::AttackStart();

	if(IsAttacking())
	{
		switch (AttackType)
		{
			case EDWAttackType::NormalAttack:
			case EDWAttackType::FallingAttack:
			{
				if(AttackAbilityQueue > 0)
				{
					AttackAbilityQueue--;
				}
				break;
			}
			default: break;
		}
	}
}

void ADWPlayerCharacter::RefreshEquip(EDWEquipPartType InPartType, UInventoryEquipSlot* EquipSlot)
{
	if(!EquipSlot->IsEmpty())
	{
		Super::RefreshEquip(InPartType, EquipSlot);
		if(Equips[InPartType])
		{
			PreviewCapture->ShowOnlyActors.Add(Equips[InPartType]);
		}
	}
	else
	{
		if(Equips[InPartType])
		{
			PreviewCapture->ShowOnlyActors.Remove(Equips[InPartType]);
		}
		Super::RefreshEquip(InPartType, EquipSlot);
	}
}

void ADWPlayerCharacter::UpdateVoxelMesh()
{
	const FAbilityItem tmpItem = UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedItem();
	if(!VoxelItem.IsValid() || !VoxelItem.EqualType(tmpItem))
	{
		if(tmpItem.IsValid() && tmpItem.GetData().EqualType(EAbilityItemType::Voxel))
		{
			VoxelItem = FVoxelItem(tmpItem.ID);
			VoxelMesh->BuildVoxel(VoxelItem);
			VoxelMesh->CreateMesh(0, false);
		}
		else
		{
			VoxelMesh->ClearMesh();
			VoxelItem = FVoxelItem::EmptyVoxel;
		}
	}
}

bool ADWPlayerCharacter::RaycastVoxel(FVoxelHitResult& OutHitResult)
{
	if(GetController<ADWPlayerController>())
	{
		FHitResult hitResult;
		if(GetController<ADWPlayerController>()->RaycastFromAimPoint(hitResult, EDWGameTraceType::Voxel, InteractDistance) && hitResult.GetActor()->IsA(AVoxelChunk::StaticClass()))
		{
			AVoxelChunk* chunk = Cast<AVoxelChunk>(hitResult.GetActor());
			if(chunk != nullptr)
			{
				const FVoxelItem& voxelItem = chunk->GetVoxelItem(chunk->LocationToIndex(hitResult.ImpactPoint - AVoxelModule::GetWorldData()->GetBlockSizedNormal(hitResult.ImpactNormal, 0.01f)));
				if(voxelItem.IsValid())
				{
					OutHitResult = FVoxelHitResult(voxelItem, hitResult.ImpactPoint, hitResult.ImpactNormal);
					return true;
				}
			}
		}
	}
	return false;
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

void ADWPlayerCharacter::ToggleControlMode()
{
	if(IsBreakAllInput() || !IsFreeToAnim()) return;

	if(ControlMode == EDWControlMode::Fighting)
	{
		SetControlMode(EDWControlMode::Creating);
	}
	else
	{
		SetControlMode(EDWControlMode::Fighting);
	}
}

void ADWPlayerCharacter::ToggleCrouch()
{
	if(IsBreakAllInput()) return;

	if(!IsCrouching())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ADWPlayerCharacter::ToggleLockTarget()
{
	if(IsBreakAllInput()) return;

	TargetSystem->TargetActor();

	// if(!LockedTarget)
	// {
	// 	TArray<FHitResult> hitResults;
	// 	if(UKismetSystemLibrary::SphereTraceMulti(this, GetActorLocation(), GetActorLocation(), 1000, UDWHelper::GetGameTrace(EGameTraceType::Sight), true, TArray<AActor*>(), EDrawDebugTrace::None, hitResults, true))
	// 	{
	// 		ADWCharacter* nearCharacter = nullptr;
	// 		for (int i = 0; i < hitResults.Num(); i++)
	// 		{
	// 			if(hitResults[i].GetActor()->IsA(ADWCharacter::StaticClass()))
	// 			{
	// 				ADWCharacter* character = Cast<ADWCharacter>(hitResults[i].GetActor());
	// 				if(!character->IsDead() && (!nearCharacter || Distance(character, false, false) < Distance(nearCharacter, false, false)))
	// 				{
	// 					nearCharacter = character;
	// 				}
	// 			}
	// 		}
	// 		if(nearCharacter)
	// 		{
	// 			SetLockedTarget(nearCharacter);
	// 		}
	// 	}
	// }
	// else
	// {
	// 	SetLockedTarget(nullptr);
	// }
}

void ADWPlayerCharacter::OnDodgePressed()
{
	if(IsBreakAllInput()) return;

	Dodge();
}

void ADWPlayerCharacter::OnDodgeReleased()
{
	//UnDodge();
}

bool ADWPlayerCharacter::UseItem(FAbilityItem& InItem)
{
	if((InItem.GetData().EqualType(EAbilityItemType::Voxel)))
	{
		if(ControlMode == EDWControlMode::Creating)
		{
			return Super::UseItem(InItem);
		}
	}
	else if((InItem.GetData().EqualType(EAbilityItemType::Prop)))
	{
		return Super::UseItem(InItem);
	}
	return false;
}

void ADWPlayerCharacter::OnAttackDestroyPressed()
{
	if(IsBreakAllInput() || UInputModuleBPLibrary::GetGlobalInputMode() != EInputMode::GameOnly) return;

	switch (ControlMode)
	{
		case EDWControlMode::Fighting:
		{
			if(GetWeapon())
			{
				bPressedAttack = true;
				if(IsFreeToAnim() || IsAttacking())
				{
					if(AttackAbilityQueue < 2)
					{
						AttackAbilityQueue++; 
					}
				}
			}
			break;
		}
		case EDWControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				voxelHitResult.GetVoxel()->OnMouseDown(EMouseButton::Left, voxelHitResult);
				// UVoxel::DespawnVoxel(voxelHitResult.GetVoxel());
			}
			break;
		}
	}
}

void ADWPlayerCharacter::OnAttackDestroyRepeat()
{
	if(IsBreakAllInput() || UInputModuleBPLibrary::GetGlobalInputMode() != EInputMode::GameOnly) return;

	switch (ControlMode)
	{
		case EDWControlMode::Fighting:
		{
			bPressedAttack = true;
			break;
		}
		case EDWControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				if(UVoxel* voxel = voxelHitResult.GetVoxel())
				{
					voxel->OnMouseHold(EMouseButton::Left, voxelHitResult);
					UVoxel::DespawnVoxel(voxel);
				}
			}
			break;
		}
	}
}

void ADWPlayerCharacter::OnAttackDestroyReleased()
{
	if(IsBreakAllInput() || UInputModuleBPLibrary::GetGlobalInputMode() != EInputMode::GameOnly) return;

	switch (ControlMode)
	{
		case EDWControlMode::Fighting:
		{
			bPressedAttack = false;
			break;
		}
		case EDWControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				if(UVoxel* voxel = voxelHitResult.GetVoxel())
				{
					voxel->OnMouseUp(EMouseButton::Left, voxelHitResult);
					UVoxel::DespawnVoxel(voxel);
				}
			}
			break;
		}
	}
}

void ADWPlayerCharacter::OnDefendGeneratePressed()
{
	if(IsBreakAllInput() || UInputModuleBPLibrary::GetGlobalInputMode() != EInputMode::GameOnly) return;

	switch (ControlMode)
	{
		case EDWControlMode::Fighting:
		{
			if(GetShield())
			{
				bPressedDefend = true;
			}
			break;
		}
		case EDWControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				if(UVoxel* voxel = voxelHitResult.GetVoxel())
				{
					voxel->OnMouseDown(EMouseButton::Right, voxelHitResult);
					UVoxel::DespawnVoxel(voxel);
				}
			}
			break;
		}
	}
}

void ADWPlayerCharacter::OnDefendGenerateRepeat()
{
	if(IsBreakAllInput() || UInputModuleBPLibrary::GetGlobalInputMode() != EInputMode::GameOnly) return;

	switch (ControlMode)
	{
		case EDWControlMode::Fighting:
		{
			bPressedDefend = true;
			break;
		}
		case EDWControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				if(UVoxel* voxel = voxelHitResult.GetVoxel())
				{
					voxel->OnMouseHold(EMouseButton::Right, voxelHitResult);
					UVoxel::DespawnVoxel(voxel);
				}
			}
			break;
		}
	}
}

void ADWPlayerCharacter::OnDefendGenerateReleased()
{
	if(IsBreakAllInput() || UInputModuleBPLibrary::GetGlobalInputMode() != EInputMode::GameOnly) return;

	switch (ControlMode)
	{
		case EDWControlMode::Fighting:
		{
			bPressedDefend = false;
			break;
		}
		case EDWControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				if(UVoxel* voxel = voxelHitResult.GetVoxel())
				{
					voxel->OnMouseUp(EMouseButton::Right, voxelHitResult);
					UVoxel::DespawnVoxel(voxel);
				}
			}
			break;
		}
	}
}

void ADWPlayerCharacter::ReleaseSkillAbility1()
{
	if(IsBreakAllInput()) return;

	if(Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill).IsValidIndex(0))
	{
		Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill)[0]->ActiveItem();
	}
}

void ADWPlayerCharacter::ReleaseSkillAbility2()
{
	if(IsBreakAllInput()) return;

	if(Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill).IsValidIndex(1))
	{
		Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill)[1]->ActiveItem();
	}
}

void ADWPlayerCharacter::ReleaseSkillAbility3()
{
	if(IsBreakAllInput()) return;

	if(Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill).IsValidIndex(2))
	{
		Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill)[2]->ActiveItem();
	}
}

void ADWPlayerCharacter::ReleaseSkillAbility4()
{
	if(IsBreakAllInput()) return;

	if(Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill).IsValidIndex(3))
	{
		Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill)[3]->ActiveItem();
	}
}

void ADWPlayerCharacter::DoInteractAction1()
{
	if(UInteractionComponent* InteractionAgent = GetInteractionComponent()->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(this).IsValidIndex(0))
		{
			InteractionAgent->DoInteract(this, InteractionAgent->GetValidInteractActions(this)[0]);
		}
	}
}

void ADWPlayerCharacter::DoInteractAction2()
{
	if(UInteractionComponent* InteractionAgent = GetInteractionComponent()->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(this).IsValidIndex(1))
		{
			InteractionAgent->DoInteract(this, InteractionAgent->GetValidInteractActions(this)[1]);
		}
	}
}

void ADWPlayerCharacter::DoInteractAction3()
{
	if(UInteractionComponent* InteractionAgent = GetInteractionComponent()->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(this).IsValidIndex(2))
		{
			InteractionAgent->DoInteract(this, InteractionAgent->GetValidInteractActions(this)[2]);
		}
	}
}

void ADWPlayerCharacter::DoInteractAction4()
{
	if(UInteractionComponent* InteractionAgent = GetInteractionComponent()->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(this).IsValidIndex(3))
		{
			InteractionAgent->DoInteract(this, InteractionAgent->GetValidInteractActions(this)[3]);
		}
	}
}

void ADWPlayerCharacter::DoInteractAction5()
{
	if(UInteractionComponent* InteractionAgent = GetInteractionComponent()->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(this).IsValidIndex(4))
		{
			InteractionAgent->DoInteract(this, InteractionAgent->GetValidInteractActions(this)[4]);
		}
	}
}

void ADWPlayerCharacter::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	Super::OnAttributeChange(InAttributeChangeData);

	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;
	if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetHealthAttribute())
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
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMaxHealthAttribute())
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
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetManaAttribute())
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
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMaxManaAttribute())
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
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMaxStaminaAttribute())
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
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMaxStaminaAttribute())
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
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMoveSpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetMoveSpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetSwimSpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetSwimSpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetRideSpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetRideSpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetFlySpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetFlySpeed(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetRotationSpeedAttribute())
	{
		GetCharacterMovement()->RotationRate = FRotator(0, InAttributeChangeData.NewValue * (IsSprinting() ? 1.5f : 1), 0) * RotationRate;
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetAttackForceAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetAttackForce(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetRepulseForceAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetRepulseForce(FString::FromInt(InAttributeChangeData.NewValue));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetAttackSpeedAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetAttackSpeed(FString::SanitizeFloat(InAttributeChangeData.NewValue, 1));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetAttackCritRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetAttackCritRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetAttackStealRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetAttackStealRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetDefendRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetDefendRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetPhysicsDefRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetPhysicsDefRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetMagicDefRateAttribute())
	{
		if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>())
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryPanel>()->SetMagicDefRate(FString::Printf(TEXT("%d%%"), (int32)(InAttributeChangeData.NewValue * 100)));
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWCharacterAttributeSet>(AttributeSet)->GetToughnessRateAttribute())
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

void ADWPlayerCharacter::SetEXP(int32 InEXP)
{
	Super::SetEXP(InEXP);
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
