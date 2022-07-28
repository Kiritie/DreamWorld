// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DWPlayerController.h"

#include "Engine/World.h"
#include "Engine.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Ability/Components/CharacterInteractionComponent.h"
#include "Ability/Components/InteractionComponent.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Camera/CameraModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Static.h"
#include "FSM/Components/FSMComponent.h"
#include "Widget/WidgetGameHUD.h"
#include "Gameplay/DWGameInstance.h"
#include "Gameplay/DWGameState.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Gameplay/DWGameMode.h"
#include "Global/GlobalBPLibrary.h"
#include "Input/InputModuleBPLibrary.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Inventory/CharacterInventory.h"
#include "Inventory/Slot/InventorySkillSlot.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Pausing.h"
#include "Procedure/Procedure_Playing.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Datas/VoxelData.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Voxels/Voxel.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"

ADWPlayerController::ADWPlayerController()
{
	// inputs
	bPressedAttack = false;
	bPressedDefend = false;
	bPressedSprint = false;
	AttackAbilityQueue = 0;
}

void ADWPlayerController::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWPlayerController::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
}

void ADWPlayerController::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	ADWPlayerCharacter* PlayerCharacter = nullptr;
	
	auto SaveData = InSaveData->CastRef<FDWPlayerSaveData>();
	if(bForceMode)
	{
		PlayerCharacter = UObjectPoolModuleBPLibrary::SpawnObject<ADWPlayerCharacter>(nullptr, SaveData.GetCharacterData().Class);
		PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, false);
		SetPlayerPawn(PlayerCharacter);
	}
	else
	{
		PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	}
	if(PlayerCharacter)
	{
		PlayerCharacter->LoadSaveData(InSaveData, bForceMode);
	}
}

FSaveData* ADWPlayerController::ToData()
{
	return nullptr;
}

void ADWPlayerController::UnloadData(bool bForceMode)
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(PlayerCharacter)
	{
		if(bForceMode)
		{
			UObjectPoolModuleBPLibrary::DespawnObject(PlayerCharacter);
			SetPlayerPawn(nullptr);
		}
		else
		{
			UnPossess();
			PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, false);
		}
	}
}

void ADWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up gameplay key bindings
	check(InputComponent);

	// FInputActionHandlerDynamicSignature InputActionHandlerSignature;
	// InputActionHandlerSignature.BindDynamic(this, &ADWPlayerController::OnJumpPressed);
	// UInputModuleBPLibrary::BindInputAction("Jump", IE_Released, InputActionHandlerSignature);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ADWPlayerController::OnJumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &ADWPlayerController::OnJumpReleased);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &ADWPlayerController::OnSprintPressed);
	InputComponent->BindAction("Sprint", IE_Released, this, &ADWPlayerController::OnSprintReleased);
	
	InputComponent->BindAction("Interact1", IE_Pressed, this, &ADWPlayerController::DoInteractAction1);
	InputComponent->BindAction("Interact2", IE_Pressed, this, &ADWPlayerController::DoInteractAction1);
	InputComponent->BindAction("Interact3", IE_Pressed, this, &ADWPlayerController::DoInteractAction1);
	InputComponent->BindAction("Interact4", IE_Pressed, this, &ADWPlayerController::DoInteractAction1);
	InputComponent->BindAction("Interact5", IE_Pressed, this, &ADWPlayerController::DoInteractAction1);

	InputComponent->BindAction("Dodge", IE_Pressed, this, &ADWPlayerController::OnDodgePressed);
	InputComponent->BindAction("Dodge", IE_Released, this, &ADWPlayerController::OnDodgeReleased);

	InputComponent->BindAction("ToggleCrouch", IE_Released, this, &ADWPlayerController::ToggleCrouch);
	InputComponent->BindAction("ToggleControlMode", IE_Pressed, this, &ADWPlayerController::ToggleControlMode);
	InputComponent->BindAction("ToggleLockSightTarget", IE_Pressed, this, &ADWPlayerController::ToggleLockTarget);

	InputComponent->BindAction("Attack/Destroy", IE_Pressed, this, &ADWPlayerController::OnAttackDestroyPressed);
	InputComponent->BindAction("Attack/Destroy", IE_Released, this, &ADWPlayerController::OnAttackDestroyReleased);

	InputComponent->BindAction("Defend/Generate", IE_Pressed, this, &ADWPlayerController::OnDefendGeneratePressed);
	InputComponent->BindAction("Defend/Generate", IE_Released, this, &ADWPlayerController::OnDefendGenerateReleased);

	InputComponent->BindAction("ReleaseSkillAbility1", IE_Pressed, this, &ADWPlayerController::ReleaseSkillAbility1);
	InputComponent->BindAction("ReleaseSkillAbility2", IE_Pressed, this, &ADWPlayerController::ReleaseSkillAbility2);
	InputComponent->BindAction("ReleaseSkillAbility3", IE_Pressed, this, &ADWPlayerController::ReleaseSkillAbility3);
	InputComponent->BindAction("ReleaseSkillAbility4", IE_Pressed, this, &ADWPlayerController::ReleaseSkillAbility4);

	InputComponent->BindAction("UseInventoryItem", IE_Pressed, this, &ADWPlayerController::UseInventoryItem);
	InputComponent->BindAction("UseAllInventoryItem", IE_Pressed, this, &ADWPlayerController::UseAllInventoryItem);
	InputComponent->BindAction("DiscardInventoryItem", IE_Pressed, this, &ADWPlayerController::DiscardInventoryItem);
	InputComponent->BindAction("DiscardAllInventoryItem", IE_Pressed, this, &ADWPlayerController::DiscardAllInventoryItem);
	InputComponent->BindAction("PrevInventorySlot", IE_Pressed, this, &ADWPlayerController::PrevInventorySlot);
	InputComponent->BindAction("NextInventorySlot", IE_Pressed, this, &ADWPlayerController::NextInventorySlot);
	InputComponent->BindAction("SelectInventorySlot1", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot1);
	InputComponent->BindAction("SelectInventorySlot2", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot2);
	InputComponent->BindAction("SelectInventorySlot3", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot3);
	InputComponent->BindAction("SelectInventorySlot4", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot4);
	InputComponent->BindAction("SelectInventorySlot5", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot5);
	InputComponent->BindAction("SelectInventorySlot6", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot6);
	InputComponent->BindAction("SelectInventorySlot7", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot7);
	InputComponent->BindAction("SelectInventorySlot8", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot8);
	InputComponent->BindAction("SelectInventorySlot9", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot9);
	InputComponent->BindAction("SelectInventorySlot10", IE_Pressed, this, &ADWPlayerController::SelectInventorySlot10);

	InputComponent->BindAction("ToggleInventoryPanel", IE_Pressed, this, &ADWPlayerController::ToggleInventoryPanel);

	InputComponent->BindAction("Pause/ContinueGame", IE_Pressed, this, &ADWPlayerController::PauseOrContinueGame);
}

void ADWPlayerController::OnPossess(APawn* InPawn)
{
	if(InPawn->IsA<ADWPlayerCharacter>() && (InPawn != GetPlayerPawn() || !GetPawn()))
	{
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGameHUD>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryBar>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryPanel>(InPawn);
	}
	Super::OnPossess(InPawn);

	if(USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
	{
		UCameraModuleBPLibrary::SetCameraDistance(USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetCameraDistance(), true);
	}
}

void ADWPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	bPressedAttack = false;
	bPressedDefend = false;
	bPressedSprint = false;
	AttackAbilityQueue = 0;
}

void ADWPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsDead()) return;

	if(PossessedCharacter->IsActive())
	{
		if(bPressedAttack)
		{
			OnAttackDestroyRepeat();
		}

		if(bPressedDefend)
		{
			OnDefendGenerateRepeat();
		}

		switch (PossessedCharacter->ControlMode)
		{
			case EDWCharacterControlMode::Fighting:
			{
				if(bPressedAttack || AttackAbilityQueue > 0)
				{
					if(PossessedCharacter->Attack() && AttackAbilityQueue > 0)
					{
						AttackAbilityQueue--;
					}
				}
				else if(PossessedCharacter->IsFreeToAnim() && PossessedCharacter->AttackType == EDWCharacterAttackType::NormalAttack)
				{
					PossessedCharacter->UnAttack();
				}

				if(bPressedDefend)
				{
					PossessedCharacter->Defend();
				}
				else
				{
					PossessedCharacter->UnDefend();
				}
			}
			case EDWCharacterControlMode::Creating:
			{
				// FVoxelHitResult voxelHitResult;
				// if(RaycastVoxel(voxelHitResult))
				// {
				// 	voxelHitResult.GetVoxel().OnMouseHover(voxelHitResult);
				// }
				break;
			}
		}

		if(bPressedSprint && PossessedCharacter->GetMoveVelocity().Size() > 0.2f)
		{
			PossessedCharacter->Sprint();
		}
		else
		{
			PossessedCharacter->UnSprint();
		}
	}
}

bool ADWPlayerController::RaycastFromAimPoint(FHitResult& OutHitResult, EDWGameTraceType InGameTraceType, float InRayDistance) const
{
	int32 viewportSizeX, viewportSizeY;
	FVector sightPos, rayDirection;
	GetViewportSize(viewportSizeX, viewportSizeY);
	if(DeprojectScreenPositionToWorld(viewportSizeX * 0.5f, viewportSizeY * 0.5f, sightPos, rayDirection))
	{
		const FVector rayStart = PlayerCameraManager->GetCameraLocation();
		const FVector rayEnd = rayStart + rayDirection * InRayDistance;
		return UKismetSystemLibrary::LineTraceSingle(this, rayStart, rayEnd, UDWHelper::GetGameTrace(InGameTraceType), false, TArray<AActor*>(), EDrawDebugTrace::None, OutHitResult, true);
	}
	return false;
}

bool ADWPlayerController::RaycastVoxel(FVoxelHitResult& OutHitResult)
{
	ADWPlayerCharacter* PlayerCharacter = GetPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return false;
	
	FHitResult hitResult;
	if(RaycastFromAimPoint(hitResult, EDWGameTraceType::Voxel, PlayerCharacter->GetInteractDistance()) && hitResult.GetActor()->IsA<AVoxelChunk>())
	{
		AVoxelChunk* chunk = Cast<AVoxelChunk>(hitResult.GetActor());
		if(chunk != nullptr)
		{
			const FVoxelItem& voxelItem = chunk->GetVoxelItem(chunk->LocationToIndex(hitResult.ImpactPoint - UVoxelModuleBPLibrary::GetWorldData().GetBlockSizedNormal(hitResult.ImpactNormal, 0.01f)));
			if(voxelItem.IsValid())
			{
				OutHitResult = FVoxelHitResult(voxelItem, hitResult.ImpactPoint, hitResult.ImpactNormal);
				return true;
			}
		}
	}
	return false;
}

void ADWPlayerController::OnJumpPressed(FKey Key)
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	PossessedCharacter->Jump();
}

void ADWPlayerController::OnJumpReleased()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	//PossessedCharacter->UnJump();
}

void ADWPlayerController::OnSprintPressed()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	bPressedSprint = true;
}

void ADWPlayerController::OnSprintReleased()
{
	bPressedSprint = false;
}

void ADWPlayerController::ToggleControlMode()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->ControlMode == EDWCharacterControlMode::Fighting)
	{
		PossessedCharacter->SetControlMode(EDWCharacterControlMode::Creating);
	}
	else
	{
		PossessedCharacter->SetControlMode(EDWCharacterControlMode::Fighting);
	}
}

void ADWPlayerController::ToggleCrouch()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(!PossessedCharacter->IsCrouching())
	{
		PossessedCharacter->Crouch(true);
	}
	else
	{
		PossessedCharacter->UnCrouch(true);
	}
}

void ADWPlayerController::ToggleLockTarget()
{
	ADWPlayerCharacter* PlayerCharacter = GetPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	PlayerCharacter->TargetSystem->TargetActor();
}

void ADWPlayerController::OnDodgePressed()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	PossessedCharacter->Dodge();
}

void ADWPlayerController::OnDodgeReleased()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	PossessedCharacter->UnDodge();
}

void ADWPlayerController::OnAttackDestroyPressed()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(PossessedCharacter->GetWeapon())
			{
				bPressedAttack = true;
				if(PossessedCharacter->IsFreeToAnim() || PossessedCharacter->IsAttacking())
				{
					if(AttackAbilityQueue < PossessedCharacter->GetAttackAbilities().Num())
					{
						AttackAbilityQueue++; 
					}
				}
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				voxelHitResult.GetVoxel().OnMouseDown(EMouseButton::Left, voxelHitResult);
			}
			break;
		}
	}
}

void ADWPlayerController::OnAttackDestroyRepeat()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			bPressedAttack = true;
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				voxelHitResult.GetVoxel().OnMouseHold(EMouseButton::Left, voxelHitResult);
			}
			break;
		}
	}
}

void ADWPlayerController::OnAttackDestroyReleased()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			bPressedAttack = false;
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				voxelHitResult.GetVoxel().OnMouseUp(EMouseButton::Left, voxelHitResult);
			}
			break;
		}
	}
}

void ADWPlayerController::OnDefendGeneratePressed()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(PossessedCharacter->GetShield())
			{
				bPressedDefend = true;
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				voxelHitResult.GetVoxel().OnMouseDown(EMouseButton::Right, voxelHitResult);
			}
			break;
		}
	}
}

void ADWPlayerController::OnDefendGenerateRepeat()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			bPressedDefend = true;
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				voxelHitResult.GetVoxel().OnMouseHold(EMouseButton::Right, voxelHitResult);
			}
			break;
		}
	}
}

void ADWPlayerController::OnDefendGenerateReleased()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			bPressedDefend = false;
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(RaycastVoxel(voxelHitResult))
			{
				voxelHitResult.GetVoxel().OnMouseUp(EMouseButton::Right, voxelHitResult);
			}
			break;
		}
	}
}

void ADWPlayerController::ReleaseSkillAbility1()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();
	
	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill).IsValidIndex(0))
	{
		PossessedCharacter->Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill)[0]->ActiveItem();
	}
}

void ADWPlayerController::ReleaseSkillAbility2()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();
	
	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill).IsValidIndex(1))
	{
		PossessedCharacter->Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill)[1]->ActiveItem();
	}
}

void ADWPlayerController::ReleaseSkillAbility3()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();
	
	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill).IsValidIndex(2))
	{
		PossessedCharacter->Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill)[2]->ActiveItem();
	}
}

void ADWPlayerController::ReleaseSkillAbility4()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();
	
	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill).IsValidIndex(3))
	{
		PossessedCharacter->Inventory->GetSplitSlots<UInventorySkillSlot>(ESplitSlotType::Skill)[3]->ActiveItem();
	}
}

void ADWPlayerController::DoInteractAction1()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractionAgent()) return;
	
	if(UInteractionComponent* InteractionAgent = PlayerCharacter->Interaction->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(0))
		{
			InteractionAgent->DoInteract(PlayerCharacter, InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[0]);
		}
	}
}

void ADWPlayerController::DoInteractAction2()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractionAgent()) return;
	
	if(UInteractionComponent* InteractionAgent = PlayerCharacter->Interaction->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(1))
		{
			InteractionAgent->DoInteract(PlayerCharacter, InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[1]);
		}
	}
}

void ADWPlayerController::DoInteractAction3()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractionAgent()) return;
	
	if(UInteractionComponent* InteractionAgent = PlayerCharacter->Interaction->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(2))
		{
			InteractionAgent->DoInteract(PlayerCharacter, InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[2]);
		}
	}
}

void ADWPlayerController::DoInteractAction4()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractionAgent()) return;
	
	if(UInteractionComponent* InteractionAgent = PlayerCharacter->Interaction->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(3))
		{
			InteractionAgent->DoInteract(PlayerCharacter, InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[3]);
		}
	}
}

void ADWPlayerController::DoInteractAction5()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractionAgent()) return;
	
	if(UInteractionComponent* InteractionAgent = PlayerCharacter->Interaction->GetInteractionAgent()->GetInteractionComponent())
	{
		if(InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(4))
		{
			InteractionAgent->DoInteract(PlayerCharacter, InteractionAgent->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[4]);
		}
	}
}

void ADWPlayerController::ToggleInventoryPanel()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::ToggleUserWidget<UWidgetInventoryPanel>(false);
	}
}

void ADWPlayerController::UseInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(1);
	}
}

void ADWPlayerController::UseAllInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(-1);
	}
}

void ADWPlayerController::DiscardInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(1);
	}
}

void ADWPlayerController::DiscardAllInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(-1);
	}
}

void ADWPlayerController::PrevInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->PrevInventorySlot();
	}
}

void ADWPlayerController::NextInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->NextInventorySlot();
	}
}

void ADWPlayerController::SelectInventorySlot1()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(0);
	}
}

void ADWPlayerController::SelectInventorySlot2()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(1);
	}
}

void ADWPlayerController::SelectInventorySlot3()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(2);
	}
}

void ADWPlayerController::SelectInventorySlot4()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(3);
	}
}

void ADWPlayerController::SelectInventorySlot5()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(4);
	}
}

void ADWPlayerController::SelectInventorySlot6()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(5);
	}
}

void ADWPlayerController::SelectInventorySlot7()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(6);
	}
}

void ADWPlayerController::SelectInventorySlot8()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(7);
	}
}

void ADWPlayerController::SelectInventorySlot9()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(8);
	}
}

void ADWPlayerController::SelectInventorySlot10()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(9);
	}
}

void ADWPlayerController::PauseOrContinueGame()
{
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Pausing>();
	}
	else if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Pausing>())
	{
		UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Playing>();
	}
}
