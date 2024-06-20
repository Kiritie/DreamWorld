// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DWInputManager.h"

#include "Ability/Character/AbilityCharacterInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Character/DWCharacter.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Common/Targeting/TargetingComponent.h"
#include "Input/InputModule.h"
#include "Input/InputModuleStatics.h"
#include "Input/Components/InputComponentBase.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/Procedure_Pausing.h"
#include "Procedure/Procedure_Playing.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Generate/WidgetGeneratePanel.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Common/DWCommonTypes.h"
#include "Scene/SceneModuleStatics.h"
#include "Widget/MaxMap/WidgetMaxMapBox.h"

// ParamSets default values
UDWInputManager::UDWInputManager()
{
	bPressedAttackDestroy = false;
	bPressedDefendGenerate = false;
	bPressedSprint = false;
}

void UDWInputManager::OnInitialize()
{
	Super::OnInitialize();
}

void UDWInputManager::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);

	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();
	
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

void UDWInputManager::OnBindAction(UInputComponentBase* InInputComponent)
{
	Super::OnBindAction(InInputComponent);

	InInputComponent->BindInputAction(GameplayTags::InputTag_Sprint, ETriggerEvent::Started, this, &UDWInputManager::OnSprintPressed);
	InInputComponent->BindInputAction(GameplayTags::InputTag_Sprint, ETriggerEvent::Completed, this, &UDWInputManager::OnSprintReleased);
	
	InInputComponent->BindInputAction(GameplayTags::InputTag_Interact1, ETriggerEvent::Started, this, &UDWInputManager::DoInteract1);
	InInputComponent->BindInputAction(GameplayTags::InputTag_Interact2, ETriggerEvent::Started, this, &UDWInputManager::DoInteract2);
	InInputComponent->BindInputAction(GameplayTags::InputTag_Interact3, ETriggerEvent::Started, this, &UDWInputManager::DoInteract3);
	InInputComponent->BindInputAction(GameplayTags::InputTag_Interact4, ETriggerEvent::Started, this, &UDWInputManager::DoInteract4);
	InInputComponent->BindInputAction(GameplayTags::InputTag_Interact5, ETriggerEvent::Started, this, &UDWInputManager::DoInteract5);
	
	InInputComponent->BindInputAction(GameplayTags::InputTag_Dodge, ETriggerEvent::Started, this, &UDWInputManager::OnDodgePressed);
	InInputComponent->BindInputAction(GameplayTags::InputTag_Dodge, ETriggerEvent::Completed, this, &UDWInputManager::OnDodgeReleased);
	
	// InInputComponent->BindInputAction(GameplayTags::InputTag_ToggleCrouch, ETriggerEvent::Started, this, &UDWInputManager::ToggleCrouch);
	InInputComponent->BindInputAction(GameplayTags::InputTag_ToggleControlMode, ETriggerEvent::Started, this, &UDWInputManager::ToggleControlMode);
	InInputComponent->BindInputAction(GameplayTags::InputTag_ToggleLockSightTarget, ETriggerEvent::Started, this, &UDWInputManager::ToggleLockTarget);
	InInputComponent->BindInputAction(GameplayTags::InputTag_ChangeHand, ETriggerEvent::Started, this, &UDWInputManager::ChangeHand);
	
	InInputComponent->BindInputAction(GameplayTags::InputTag_Primary, ETriggerEvent::Started, this, &UDWInputManager::OnPrimaryPressed);
	InInputComponent->BindInputAction(GameplayTags::InputTag_Primary, ETriggerEvent::Completed, this, &UDWInputManager::OnPrimaryReleased);
	
	InInputComponent->BindInputAction(GameplayTags::InputTag_Secondary, ETriggerEvent::Started, this, &UDWInputManager::OnSecondaryPressed);
	InInputComponent->BindInputAction(GameplayTags::InputTag_Secondary, ETriggerEvent::Completed, this, &UDWInputManager::OnSecondaryReleased);
	
	InInputComponent->BindInputAction(GameplayTags::InputTag_ReleaseSkillAbility1, ETriggerEvent::Started, this, &UDWInputManager::ReleaseSkillAbility1);
	InInputComponent->BindInputAction(GameplayTags::InputTag_ReleaseSkillAbility2, ETriggerEvent::Started, this, &UDWInputManager::ReleaseSkillAbility2);
	InInputComponent->BindInputAction(GameplayTags::InputTag_ReleaseSkillAbility3, ETriggerEvent::Started, this, &UDWInputManager::ReleaseSkillAbility3);
	InInputComponent->BindInputAction(GameplayTags::InputTag_ReleaseSkillAbility4, ETriggerEvent::Started, this, &UDWInputManager::ReleaseSkillAbility4);
	
	InInputComponent->BindInputAction(GameplayTags::InputTag_UseInventoryItem, ETriggerEvent::Started, this, &UDWInputManager::UseInventoryItem);
	InInputComponent->BindInputAction(GameplayTags::InputTag_DiscardInventoryItem, ETriggerEvent::Started, this, &UDWInputManager::DiscardInventoryItem);
	InInputComponent->BindInputAction(GameplayTags::InputTag_PrevInventorySlot, ETriggerEvent::Started, this, &UDWInputManager::PrevInventorySlot);
	InInputComponent->BindInputAction(GameplayTags::InputTag_NextInventorySlot, ETriggerEvent::Started, this, &UDWInputManager::NextInventorySlot);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot1, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot1);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot2, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot2);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot3, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot3);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot4, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot4);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot5, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot5);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot6, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot6);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot7, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot7);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot8, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot8);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot9, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot9);
	InInputComponent->BindInputAction(GameplayTags::InputTag_SelectInventorySlot10, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot10);
	
	InInputComponent->BindInputAction(GameplayTags::InputTag_ZoomInMiniMap, ETriggerEvent::Started, this, &UDWInputManager::ZoomInMiniMap);
	InInputComponent->BindInputAction(GameplayTags::InputTag_ZoomOutMiniMap, ETriggerEvent::Started, this, &UDWInputManager::ZoomOutMiniMap);
	InInputComponent->BindInputAction(GameplayTags::InputTag_OpenMaxMapBox, ETriggerEvent::Started, this, &UDWInputManager::OpenMaxMapBox);
	InInputComponent->BindInputAction(GameplayTags::InputTag_OpenInventoryPanel, ETriggerEvent::Started, this, &UDWInputManager::OpenInventoryPanel);
	InInputComponent->BindInputAction(GameplayTags::InputTag_OpenGeneratePanel, ETriggerEvent::Started, this, &UDWInputManager::OpenGeneratePanel);
}

void UDWInputManager::TurnPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_Turn(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::MoveHPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_MoveH(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::MoveVPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_MoveV(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::MoveForwardPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_MoveForward(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::MoveRightPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_MoveRight(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::MoveUpPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_MoveUp(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::ActionUpPlayer_Implementation(const FInputActionValue& InValue)
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(InValue.Get<float>() > 0.f)
	{
		if(PossessedCharacter->IsWalking() || PossessedCharacter->IsFloating())
		{
			PossessedCharacter->Jump();
		}
		else if(PossessedCharacter->IsFalling())
		{
			PossessedCharacter->Fly();
		}
	}
}

void UDWInputManager::SystemOperation_Implementation()
{
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_Pausing>();
	}
}

void UDWInputManager::OnSprintPressed()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	bPressedSprint = true;
}

void UDWInputManager::OnSprintReleased()
{
	bPressedSprint = false;
}

void UDWInputManager::OnDodgePressed()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	PossessedCharacter->Dodge();
}

void UDWInputManager::OnDodgeReleased()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	// PossessedCharacter->UnDodge();
}

void UDWInputManager::OnPrimaryPressed()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

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
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, voxelHitResult))
			{
				PlayerCharacter->OnInteractVoxel(voxelHitResult, EInputInteractAction::Primary);
			}
			break;
		}
	}
}

void UDWInputManager::OnPrimaryReleased()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

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

void UDWInputManager::OnSecondaryPressed()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

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
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, voxelHitResult))
			{
				PlayerCharacter->OnInteractVoxel(voxelHitResult, EInputInteractAction::Secondary);
			}
			break;
		}
	}
}

void UDWInputManager::OnSecondaryReleased()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

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

void UDWInputManager::ToggleControlMode()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

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

void UDWInputManager::ToggleCrouch()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

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

void UDWInputManager::ToggleLockTarget()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	PlayerCharacter->GetTargeting()->TargetActor();
}

void UDWInputManager::ChangeHand()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	PlayerCharacter->ChangeHand();
}

void UDWInputManager::ReleaseSkillAbility1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill).IsValidIndex(0))
	{
		PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill)[0]->ActiveItem();
	}
}

void UDWInputManager::ReleaseSkillAbility2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill).IsValidIndex(1))
	{
		PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill)[1]->ActiveItem();
	}
}

void UDWInputManager::ReleaseSkillAbility3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill).IsValidIndex(2))
	{
		PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill)[2]->ActiveItem();
	}
}

void UDWInputManager::ReleaseSkillAbility4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill).IsValidIndex(3))
	{
		PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill)[3]->ActiveItem();
	}
}

void UDWInputManager::DoInteract1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(0))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[0]);
	}
}

void UDWInputManager::DoInteract2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(1))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[1]);
	}
}

void UDWInputManager::DoInteract3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(2))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[2]);
	}
}

void UDWInputManager::DoInteract4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(3))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[3]);
	}
}

void UDWInputManager::DoInteract5()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(4))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[4]);
	}
}

void UDWInputManager::UseInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(UInputModuleStatics::GetKeyShortcut(GameplayTags::InputTag_InventoryAll).IsPressing(UCommonStatics::GetPlayerController()) ? -1 : 1);
	}
}

void UDWInputManager::DiscardInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(UInputModuleStatics::GetKeyShortcut(GameplayTags::InputTag_InventoryAll).IsPressing(UCommonStatics::GetPlayerController()) ? -1 : 1, false);
	}
}

void UDWInputManager::PrevInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || UInputModuleStatics::GetKeyShortcut(GameplayTags::InputTag_CameraZoom).IsPressing(UCommonStatics::GetPlayerController())) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->PrevInventorySlot();
	}
}

void UDWInputManager::NextInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || UInputModuleStatics::GetKeyShortcut(GameplayTags::InputTag_CameraZoom).IsPressing(UCommonStatics::GetPlayerController())) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->NextInventorySlot();
	}
}

void UDWInputManager::SelectInventorySlot1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(0);
	}
}

void UDWInputManager::SelectInventorySlot2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(1);
	}
}

void UDWInputManager::SelectInventorySlot3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(2);
	}
}

void UDWInputManager::SelectInventorySlot4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(3);
	}
}

void UDWInputManager::SelectInventorySlot5()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(4);
	}
}

void UDWInputManager::SelectInventorySlot6()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(5);
	}
}

void UDWInputManager::SelectInventorySlot7()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(6);
	}
}

void UDWInputManager::SelectInventorySlot8()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(7);
	}
}

void UDWInputManager::SelectInventorySlot9()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(8);
	}
}

void UDWInputManager::SelectInventorySlot10()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(9);
	}
}

void UDWInputManager::ZoomInMiniMap()
{
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		USceneModuleStatics::SetMiniMapRange(USceneModuleStatics::GetMiniMapRange() + 1000.f);
	}
}

void UDWInputManager::ZoomOutMiniMap()
{
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		USceneModuleStatics::SetMiniMapRange(USceneModuleStatics::GetMiniMapRange() - 1000.f);
	}
}

void UDWInputManager::OpenMaxMapBox()
{
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::OpenUserWidget<UWidgetMaxMapBox>();
	}
}

void UDWInputManager::OpenInventoryPanel()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::OpenUserWidget<UWidgetInventoryPanel>();
	}
}

void UDWInputManager::OpenGeneratePanel()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		if(AVoxelInteractAuxiliary* InteractionAgent = PlayerCharacter->GetInteractingAgent<AVoxelInteractAuxiliary>())
		{
			switch(InteractionAgent->GetVoxelItem().GetVoxelType())
			{
				case EVoxelType::Furnace:
				case EVoxelType::Crafting_Table:
				{
					PlayerCharacter->DoInteract((EInteractAction)EVoxelInteractAction::Open, InteractionAgent);
					break;
				}
				default:
				{
					UWidgetModuleStatics::OpenUserWidget<UWidgetGeneratePanel>();
					break;
				}
			}
		}
		else
		{
			UWidgetModuleStatics::OpenUserWidget<UWidgetGeneratePanel>();
		}
	}
}
