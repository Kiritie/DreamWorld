// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DWInputModule.h"

#include "Ability/Character/AbilityCharacterInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Camera/CameraModule.h"
#include "Character/DWCharacter.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonBPLibrary.h"
#include "Common/Targeting/TargetingComponent.h"
#include "Input/InputModule.h"
#include "Input/InputModuleBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Pausing.h"
#include "Procedure/Procedure_Playing.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Widget/WidgetGeneratePanel.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"

IMPLEMENTATION_MODULE(ADWInputModule)

// ParamSets default values
ADWInputModule::ADWInputModule()
{
	bPressedAttackDestroy = false;
	bPressedDefendGenerate = false;
	bPressedSprint = false;

	ActionMappings.Add(FInputActionMapping("Jump", IE_Pressed, this, "OnJumpPressed"));
	ActionMappings.Add(FInputActionMapping("Jump", IE_Released, this, "OnJumpReleased"));
	ActionMappings.Add(FInputActionMapping("Sprint", IE_Pressed, this, "OnSprintPressed"));
	ActionMappings.Add(FInputActionMapping("Sprint", IE_Released, this, "OnSprintReleased"));
	
	ActionMappings.Add(FInputActionMapping("Interact1", IE_Pressed, this, "DoInteractAction1"));
	ActionMappings.Add(FInputActionMapping("Interact2", IE_Pressed, this, "DoInteractAction2"));
	ActionMappings.Add(FInputActionMapping("Interact3", IE_Pressed, this, "DoInteractAction3"));
	ActionMappings.Add(FInputActionMapping("Interact4", IE_Pressed, this, "DoInteractAction4"));
	ActionMappings.Add(FInputActionMapping("Interact5", IE_Pressed, this, "DoInteractAction5"));

	ActionMappings.Add(FInputActionMapping("Dodge", IE_Pressed, this, "OnDodgePressed"));
	ActionMappings.Add(FInputActionMapping("Dodge", IE_Released, this, "OnDodgeReleased"));

	ActionMappings.Add(FInputActionMapping("ToggleCrouch", IE_Released, this, "ToggleCrouch"));
	ActionMappings.Add(FInputActionMapping("ToggleControlMode", IE_Pressed, this, "ToggleControlMode"));
	ActionMappings.Add(FInputActionMapping("ToggleLockSightTarget", IE_Pressed, this, "ToggleLockTarget"));
	ActionMappings.Add(FInputActionMapping("ChangeHand", IE_Pressed, this, "ChangeHand"));

	ActionMappings.Add(FInputActionMapping("Attack/Destroy", IE_Pressed, this, "OnAttackDestroyPressed"));
	ActionMappings.Add(FInputActionMapping("Attack/Destroy", IE_Released, this, "OnAttackDestroyReleased"));

	ActionMappings.Add(FInputActionMapping("Defend/Generate", IE_Pressed, this, "OnDefendGeneratePressed"));
	ActionMappings.Add(FInputActionMapping("Defend/Generate", IE_Released, this, "OnDefendGenerateReleased"));

	ActionMappings.Add(FInputActionMapping("ReleaseSkillAbility1", IE_Pressed, this, "ReleaseSkillAbility1"));
	ActionMappings.Add(FInputActionMapping("ReleaseSkillAbility2", IE_Pressed, this, "ReleaseSkillAbility2"));
	ActionMappings.Add(FInputActionMapping("ReleaseSkillAbility3", IE_Pressed, this, "ReleaseSkillAbility3"));
	ActionMappings.Add(FInputActionMapping("ReleaseSkillAbility4", IE_Pressed, this, "ReleaseSkillAbility4"));

	ActionMappings.Add(FInputActionMapping("UseInventoryItem", IE_Pressed, this, "UseInventoryItem"));
	ActionMappings.Add(FInputActionMapping("UseAllInventoryItem", IE_Pressed, this, "UseAllInventoryItem"));
	ActionMappings.Add(FInputActionMapping("DiscardInventoryItem", IE_Pressed, this, "DiscardInventoryItem"));
	ActionMappings.Add(FInputActionMapping("DiscardAllInventoryItem", IE_Pressed, this, "DiscardAllInventoryItem"));
	ActionMappings.Add(FInputActionMapping("PrevInventorySlot", IE_Pressed, this, "PrevInventorySlot"));
	ActionMappings.Add(FInputActionMapping("NextInventorySlot", IE_Pressed, this, "NextInventorySlot"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot1", IE_Pressed, this, "SelectInventorySlot1"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot2", IE_Pressed, this, "SelectInventorySlot2"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot3", IE_Pressed, this, "SelectInventorySlot3"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot4", IE_Pressed, this, "SelectInventorySlot4"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot5", IE_Pressed, this, "SelectInventorySlot5"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot6", IE_Pressed, this, "SelectInventorySlot6"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot7", IE_Pressed, this, "SelectInventorySlot7"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot8", IE_Pressed, this, "SelectInventorySlot8"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot9", IE_Pressed, this, "SelectInventorySlot9"));
	ActionMappings.Add(FInputActionMapping("SelectInventorySlot10", IE_Pressed, this, "SelectInventorySlot10"));

	ActionMappings.Add(FInputActionMapping("OpenInventoryPanel", IE_Pressed, this, "OpenInventoryPanel"));
	
	ActionMappings.Add(FInputActionMapping("OpenGeneratePanel", IE_Pressed, this, "OpenGeneratePanel"));

	ActionMappings.Add(FInputActionMapping("PauseGame", IE_Pressed, this, "PauseGame"));
}

#if WITH_EDITOR
void ADWInputModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWInputModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWInputModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWInputModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWInputModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);

	ADWCharacter* PossessedCharacter = UCommonBPLibrary::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(bPressedSprint)
	{
		PossessedCharacter->Sprint();
	}
	else
	{
		PossessedCharacter->UnSprint();
	}

	if(ADWPlayerCharacter* PlayerCharacter = Cast<ADWPlayerCharacter>(PossessedCharacter))
	{
		switch (PlayerCharacter->ControlMode)
		{
			case EDWCharacterControlMode::Fighting:
			{
				if(bPressedAttackDestroy || PlayerCharacter->AttackAbilityQueue > 0)
				{
					PlayerCharacter->Attack();
				}
				else if(!PlayerCharacter->IsAttacking(true))
				{
					PlayerCharacter->UnAttack();
				}

				if(bPressedDefendGenerate)
				{
					PlayerCharacter->Defend();
				}
				else
				{
					PlayerCharacter->UnDefend();
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
	}
}

void ADWInputModule::OnReset_Implementation()
{
	Super::OnReset_Implementation();
}

void ADWInputModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();

	bPressedAttackDestroy = false;
	bPressedDefendGenerate = false;
	bPressedSprint = false;
}

void ADWInputModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWInputModule::OnJumpPressed()
{
	ADWCharacter* PossessedCharacter = UCommonBPLibrary::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	PossessedCharacter->Jump();
}

void ADWInputModule::OnJumpReleased()
{
	ADWCharacter* PossessedCharacter = UCommonBPLibrary::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	// PossessedCharacter->UnJump();
}

void ADWInputModule::OnSprintPressed()
{
	ADWCharacter* PossessedCharacter = UCommonBPLibrary::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	bPressedSprint = true;
}

void ADWInputModule::OnSprintReleased()
{
	bPressedSprint = false;
}

void ADWInputModule::ToggleControlMode()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->ControlMode == EDWCharacterControlMode::Fighting)
	{
		PlayerCharacter->SetControlMode(EDWCharacterControlMode::Creating);
	}
	else
	{
		PlayerCharacter->SetControlMode(EDWCharacterControlMode::Fighting);
	}
}

void ADWInputModule::ToggleCrouch()
{
	ADWCharacter* PossessedCharacter = UCommonBPLibrary::GetPossessedPawn<ADWCharacter>();

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

void ADWInputModule::ToggleLockTarget()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	PlayerCharacter->GetTargeting()->TargetActor();
}

void ADWInputModule::ChangeHand()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	PlayerCharacter->ChangeHand();
}

void ADWInputModule::OnDodgePressed()
{
	ADWCharacter* PossessedCharacter = UCommonBPLibrary::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	PossessedCharacter->Dodge();
}

void ADWInputModule::OnDodgeReleased()
{
	ADWCharacter* PossessedCharacter = UCommonBPLibrary::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	// PossessedCharacter->UnDodge();
}

void ADWInputModule::OnAttackDestroyPressed()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	bPressedAttackDestroy = true;
	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(PlayerCharacter->IsFreeToAnim() || PlayerCharacter->IsAttacking())
			{
				if(PlayerCharacter->AttackAbilityQueue < PlayerCharacter->GetAttackAbilities().Num())
				{
					PlayerCharacter->AttackAbilityQueue++; 
				}
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(UVoxelModuleBPLibrary::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, voxelHitResult))
			{
				PlayerCharacter->OnInteractVoxel(voxelHitResult, EInputInteractAction::Action1);
			}
			break;
		}
	}
}

void ADWInputModule::OnAttackDestroyReleased()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	bPressedAttackDestroy = false;
	switch (PlayerCharacter->ControlMode)
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

void ADWInputModule::OnDefendGeneratePressed()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	bPressedDefendGenerate = true;
	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult voxelHitResult;
			if(UVoxelModuleBPLibrary::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, voxelHitResult))
			{
				PlayerCharacter->OnInteractVoxel(voxelHitResult, EInputInteractAction::Action2);
			}
			break;
		}
	}
}

void ADWInputModule::OnDefendGenerateReleased()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	bPressedDefendGenerate = false;
	switch (PlayerCharacter->ControlMode)
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

void ADWInputModule::ReleaseSkillAbility1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill).IsValidIndex(0))
	{
		PlayerCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill)[0]->ActiveItem();
	}
}

void ADWInputModule::ReleaseSkillAbility2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill).IsValidIndex(1))
	{
		PlayerCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill)[1]->ActiveItem();
	}
}

void ADWInputModule::ReleaseSkillAbility3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill).IsValidIndex(2))
	{
		PlayerCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill)[2]->ActiveItem();
	}
}

void ADWInputModule::ReleaseSkillAbility4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill).IsValidIndex(3))
	{
		PlayerCharacter->Inventory->GetSplitSlots(ESplitSlotType::Skill)[3]->ActiveItem();
	}
}

void ADWInputModule::DoInteractAction1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(0))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[0]);
	}
}

void ADWInputModule::DoInteractAction2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(1))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[1]);
	}
}

void ADWInputModule::DoInteractAction3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(2))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[2]);
	}
}

void ADWInputModule::DoInteractAction4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(3))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[3]);
	}
}

void ADWInputModule::DoInteractAction5()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(4))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[4]);
	}
}

void ADWInputModule::OpenInventoryPanel()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetInventoryPanel>(false);
	}
}

void ADWInputModule::OpenGeneratePanel()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::OpenUserWidget<UWidgetGeneratePanel>();
	}
}

void ADWInputModule::UseInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(1);
	}
}

void ADWInputModule::UseAllInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(-1);
	}
}

void ADWInputModule::DiscardInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(1, false);
	}
}

void ADWInputModule::DiscardAllInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(-1, false);
	}
}

void ADWInputModule::PrevInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || GetPlayerController()->IsInputKeyDown(UInputModuleBPLibrary::GetKeyShortcutByName(FName("CameraZoom")).Key)) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->PrevInventorySlot();
	}
}

void ADWInputModule::NextInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || GetPlayerController()->IsInputKeyDown(UInputModuleBPLibrary::GetKeyShortcutByName(FName("CameraZoom")).Key)) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->NextInventorySlot();
	}
}

void ADWInputModule::SelectInventorySlot1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(0);
	}
}

void ADWInputModule::SelectInventorySlot2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(1);
	}
}

void ADWInputModule::SelectInventorySlot3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(2);
	}
}

void ADWInputModule::SelectInventorySlot4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(3);
	}
}

void ADWInputModule::SelectInventorySlot5()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(4);
	}
}

void ADWInputModule::SelectInventorySlot6()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(5);
	}
}

void ADWInputModule::SelectInventorySlot7()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(6);
	}
}

void ADWInputModule::SelectInventorySlot8()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(7);
	}
}

void ADWInputModule::SelectInventorySlot9()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(8);
	}
}

void ADWInputModule::SelectInventorySlot10()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonBPLibrary::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(9);
	}
}

void ADWInputModule::PauseGame()
{
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Pausing>();
	}
}
