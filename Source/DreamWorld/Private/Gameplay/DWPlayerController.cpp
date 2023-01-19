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
#include "Ability/Inventory/Slot/InventorySlot.h"
#include "Ability/Inventory/CharacterInventory.h"
#include "Ability/Inventory/Slot/InventorySkillSlot.h"
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
#include "Camera/CameraModule.h"
#include "Widget/WidgetGeneratePanel.h"
#include "Widget/WidgetShopPanel.h"
#include "Procedure/Procedure_Starting.h"

ADWPlayerController::ADWPlayerController()
{
	// inputs
	bPressedAttackDestroy = false;
	bPressedDefendGenerate = false;
	bPressedSprint = false;
}

void ADWPlayerController::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWPlayerController::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWPlayerController::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);


	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter) return;

	if(PossessedCharacter->IsActive(true))
	{
		switch (PossessedCharacter->ControlMode)
		{
			case EDWCharacterControlMode::Fighting:
			{
				if(bPressedAttackDestroy || PossessedCharacter->AttackAbilityQueue > 0)
				{
					PossessedCharacter->Attack();
				}
				else if(!PossessedCharacter->IsAttacking(true))
				{
					PossessedCharacter->UnAttack();
				}

				if(bPressedDefendGenerate)
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

		if(bPressedSprint)
		{
			PossessedCharacter->Sprint();
		}
		else
		{
			PossessedCharacter->UnSprint();
		}
	}
}

void ADWPlayerController::OnTermination_Implementation()
{
	Super::OnTermination_Implementation();
}

void ADWPlayerController::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWPlayerSaveData>();

	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	switch(InPhase)
	{
		case EPhase::Primary:
		{
			bool bNeedSpawn = true;
			if(PlayerCharacter)
			{
				if(PlayerCharacter->GetAssetID() == SaveData.ID)
				{
					bNeedSpawn = false;
				}
				SaveData.SpawnLocation = PlayerCharacter->GetActorLocation();
			}
			if(bNeedSpawn)
			{
				UnloadData(InPhase);
				PlayerCharacter = UObjectPoolModuleBPLibrary::SpawnObject<ADWPlayerCharacter>({ FParameter::MakePointer(&SaveData.ID) }, SaveData.GetCharacterData().Class);
				if(PlayerCharacter)
				{
					if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Starting>())
					{
						PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, false);
					}
					SetPlayerPawn(PlayerCharacter);
				}
			}
			break;
		}
		case EPhase::Lesser:
		case EPhase::Final:
		{
			break;
		}
	}
	if(PlayerCharacter)
	{
		PlayerCharacter->LoadSaveData(&SaveData, InPhase, true);
	}
}

FSaveData* ADWPlayerController::ToData()
{
	return nullptr;
}

void ADWPlayerController::UnloadData(EPhase InPhase)
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	if(PlayerCharacter)
	{
		switch(InPhase)
		{
			case EPhase::Primary:
			{
				UObjectPoolModuleBPLibrary::DespawnObject(PlayerCharacter);
				SetPlayerPawn(nullptr);
				break;
			}
			case EPhase::Lesser:
			case EPhase::Final:
			{
				UnPossess();
				PlayerCharacter->Execute_SetActorVisible(PlayerCharacter, false);
				break;
			}
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
	InputComponent->BindAction("ChangeHand", IE_Pressed, this, &ADWPlayerController::ChangeHand);

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

	InputComponent->BindAction("OpenInventoryPanel", IE_Pressed, this, &ADWPlayerController::OpenInventoryPanel);
	
	InputComponent->BindAction("OpenGeneratePanel", IE_Pressed, this, &ADWPlayerController::OpenGeneratePanel);

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ADWPlayerController::PauseGame);
}

void ADWPlayerController::OnPossess(APawn* InPawn)
{
	if(InPawn->IsA<ADWPlayerCharacter>() && (InPawn != GetPlayerPawn() || !GetPawn()))
	{
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGameHUD>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryBar>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryPanel>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGeneratePanel>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetShopPanel>(InPawn);
	}
	Super::OnPossess(InPawn);
}

void ADWPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	bPressedAttackDestroy = false;
	bPressedDefendGenerate = false;
	bPressedSprint = false;
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

	// PossessedCharacter->UnJump();
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

void ADWPlayerController::ChangeHand()
{
	ADWPlayerCharacter* PlayerCharacter = GetPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	PlayerCharacter->ChangeHand();
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

	// PossessedCharacter->UnDodge();
}

void ADWPlayerController::OnAttackDestroyPressed()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	bPressedAttackDestroy = true;
	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(PossessedCharacter->IsFreeToAnim() || PossessedCharacter->IsAttacking())
			{
				if(PossessedCharacter->AttackAbilityQueue < PossessedCharacter->GetAttackAbilities().Num())
				{
					PossessedCharacter->AttackAbilityQueue++; 
				}
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(UVoxelModuleBPLibrary::VoxelRaycastSinge(PossessedCharacter->GetInteractDistance(), {}, voxelHitResult))
			{
				voxelHitResult.GetVoxel().OnActionTrigger(PossessedCharacter, EVoxelActionType::Action1, voxelHitResult);
			}
			break;
		}
	}
}

void ADWPlayerController::OnAttackDestroyReleased()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	bPressedAttackDestroy = false;
	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			break;
		}
	}
}

void ADWPlayerController::OnDefendGeneratePressed()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	bPressedDefendGenerate = true;
	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(UVoxelModuleBPLibrary::VoxelRaycastSinge(PossessedCharacter->GetInteractDistance(), {}, voxelHitResult))
			{
				voxelHitResult.GetVoxel().OnActionTrigger(PossessedCharacter, EVoxelActionType::Action2, voxelHitResult);
			}
			break;
		}
	}
}

void ADWPlayerController::OnDefendGenerateReleased()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	bPressedDefendGenerate = false;
	switch (PossessedCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			break;
		}
	}
}

void ADWPlayerController::ReleaseSkillAbility1()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();
	
	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill).IsValidIndex(0))
	{
		PossessedCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill)[0]->ActiveItem();
	}
}

void ADWPlayerController::ReleaseSkillAbility2()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();
	
	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill).IsValidIndex(1))
	{
		PossessedCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill)[1]->ActiveItem();
	}
}

void ADWPlayerController::ReleaseSkillAbility3()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();
	
	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill).IsValidIndex(2))
	{
		PossessedCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill)[2]->ActiveItem();
	}
}

void ADWPlayerController::ReleaseSkillAbility4()
{
	ADWCharacter* PossessedCharacter = GetPawn<ADWCharacter>();
	
	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill).IsValidIndex(3))
	{
		PossessedCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill)[3]->ActiveItem();
	}
}

void ADWPlayerController::DoInteractAction1()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractingAgent()) return;
	
	if(UInteractionComponent* Interaction = PlayerCharacter->Interaction->GetInteractingComponent())
	{
		if(Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(0))
		{
			Interaction->DoInteract(PlayerCharacter, Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[0]);
		}
	}
}

void ADWPlayerController::DoInteractAction2()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractingAgent()) return;
	
	if(UInteractionComponent* Interaction = PlayerCharacter->Interaction->GetInteractingComponent())
	{
		if(Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(1))
		{
			Interaction->DoInteract(PlayerCharacter, Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[1]);
		}
	}
}

void ADWPlayerController::DoInteractAction3()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractingAgent()) return;
	
	if(UInteractionComponent* Interaction = PlayerCharacter->Interaction->GetInteractingComponent())
	{
		if(Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(2))
		{
			Interaction->DoInteract(PlayerCharacter, Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[2]);
		}
	}
}

void ADWPlayerController::DoInteractAction4()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractingAgent()) return;
	
	if(UInteractionComponent* Interaction = PlayerCharacter->Interaction->GetInteractingComponent())
	{
		if(Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(3))
		{
			Interaction->DoInteract(PlayerCharacter, Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[3]);
		}
	}
}

void ADWPlayerController::DoInteractAction5()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->Interaction->GetInteractingAgent()) return;
	
	if(UInteractionComponent* Interaction = PlayerCharacter->Interaction->GetInteractingComponent())
	{
		if(Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>()).IsValidIndex(4))
		{
			Interaction->DoInteract(PlayerCharacter, Interaction->GetValidInteractActions(GetPlayerPawn<ADWPlayerCharacter>())[4]);
		}
	}
}

void ADWPlayerController::OpenInventoryPanel()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryPanel>(false);
	}
}

void ADWPlayerController::OpenGeneratePanel()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetGeneratePanel>(false);
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
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(1, false);
	}
}

void ADWPlayerController::DiscardAllInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(-1, false);
	}
}

void ADWPlayerController::PrevInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || UInputModuleBPLibrary::IsInputKeyDown(ACameraModule::Get()->GetCameraZoomKey())) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->PrevInventorySlot();
	}
}

void ADWPlayerController::NextInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || UInputModuleBPLibrary::IsInputKeyDown(ACameraModule::Get()->GetCameraZoomKey())) return;
	
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

void ADWPlayerController::PauseGame()
{
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Pausing>();
	}
}
