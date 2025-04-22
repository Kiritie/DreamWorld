// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DWInputManager.h"

#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Ability/Projectile/AbilityProjectileBase.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterPart.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Common/Targeting/TargetingComponent.h"
#include "Input/InputModule.h"
#include "Input/InputModuleStatics.h"
#include "Input/Components/InputComponentBase.h"
#include "Procedure/ProcedureModuleStatics.h"
#include "Procedure/DWProcedure_Pausing.h"
#include "Procedure/DWProcedure_Playing.h"
#include "Voxel/VoxelModuleStatics.h"
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Generate/WidgetGeneratePanel.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Common/DWCommonTypes.h"
#include "Procedure/DWProcedure_Testing.h"
#include "Scene/SceneModuleStatics.h"
#include "Widget/Context/WidgetContextInputBox.h"
#include "Widget/MaxMap/WidgetMaxMapBox.h"
#include "Widget/Setting/WidgetSettingPanel.h"
#include "Widget/Switcher/WidgetSwitcherPanel.h"
#include "Widget/Task/WidgetTaskPanel.h"

// ParamSets default values
UDWInputManager::UDWInputManager()
{
	bPrimaryPressed = false;
	bSecondaryPressed = false;
	bThirdPressed = false;
	bSprintPressed = false;
	AttackAbilityQueue = 0;
}

void UDWInputManager::OnInitialize()
{
	Super::OnInitialize();
}

void UDWInputManager::OnReset()
{
	Super::OnReset();

	bPrimaryPressed = false;
	bSecondaryPressed = false;
	bThirdPressed = false;
	bSprintPressed = false;
	AttackAbilityQueue = 0;
}

void UDWInputManager::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);

	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);
	
	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	if(bSprintPressed)
	{
		PossessedCharacter->Sprint();
	}
	else
	{
		PossessedCharacter->UnSprint();
	}

	ADWPlayerCharacter* PlayerCharacter = Cast<ADWPlayerCharacter>(PossessedCharacter);

	if(!PlayerCharacter) return;

	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(PlayerCharacter->IsExhausted())
			{
				AttackAbilityQueue = 0;
			}

			if(bSecondaryPressed)
			{
				if(PlayerCharacter->GetWeaponProjectile(EDWWeaponPart::Primary))
				{
					PlayerCharacter->Aim();
				}
				else
				{
					PlayerCharacter->Defend();
				}
			}
			else
			{
				PlayerCharacter->UnAim();
				PlayerCharacter->UnDefend();
			}

			if(bPrimaryPressed || AttackAbilityQueue > 0)
			{
				if(PlayerCharacter->Attack(EDWWeaponPart::Primary) && AttackAbilityQueue > 0)
				{
					AttackAbilityQueue--;
				}
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			AttackAbilityQueue = 0;
			break;
		}
	}
}

void UDWInputManager::OnBindAction(UInputComponentBase* InInputComponent)
{
	Super::OnBindAction(InInputComponent);

	InInputComponent->BindInputAction(GameplayTags::Input_Sprint, ETriggerEvent::Started, this, &UDWInputManager::OnSprintPressed);
	InInputComponent->BindInputAction(GameplayTags::Input_Sprint, ETriggerEvent::Completed, this, &UDWInputManager::OnSprintReleased);
	
	InInputComponent->BindInputAction(GameplayTags::Input_Interact1, ETriggerEvent::Started, this, &UDWInputManager::DoInteract1);
	InInputComponent->BindInputAction(GameplayTags::Input_Interact2, ETriggerEvent::Started, this, &UDWInputManager::DoInteract2);
	InInputComponent->BindInputAction(GameplayTags::Input_Interact3, ETriggerEvent::Started, this, &UDWInputManager::DoInteract3);
	InInputComponent->BindInputAction(GameplayTags::Input_Interact4, ETriggerEvent::Started, this, &UDWInputManager::DoInteract4);
	InInputComponent->BindInputAction(GameplayTags::Input_Interact5, ETriggerEvent::Started, this, &UDWInputManager::DoInteract5);
	
	InInputComponent->BindInputAction(GameplayTags::Input_NextInteract, ETriggerEvent::Started, this, &UDWInputManager::NextInteract);
	
	InInputComponent->BindInputAction(GameplayTags::Input_Dodge, ETriggerEvent::Started, this, &UDWInputManager::OnDodgePressed);
	InInputComponent->BindInputAction(GameplayTags::Input_Dodge, ETriggerEvent::Completed, this, &UDWInputManager::OnDodgeReleased);
	
	// InInputComponent->BindInputAction(GameplayTags::Input_ToggleCrouch, ETriggerEvent::Started, this, &UDWInputManager::ToggleCrouch);
	InInputComponent->BindInputAction(GameplayTags::Input_ChangeHand, ETriggerEvent::Started, this, &UDWInputManager::ChangeHand);

	InInputComponent->BindInputAction(GameplayTags::Input_ReleaseSkillAbility1, ETriggerEvent::Started, this, &UDWInputManager::ReleaseSkillAbility1);
	InInputComponent->BindInputAction(GameplayTags::Input_ReleaseSkillAbility2, ETriggerEvent::Started, this, &UDWInputManager::ReleaseSkillAbility2);
	InInputComponent->BindInputAction(GameplayTags::Input_ReleaseSkillAbility3, ETriggerEvent::Started, this, &UDWInputManager::ReleaseSkillAbility3);
	InInputComponent->BindInputAction(GameplayTags::Input_ReleaseSkillAbility4, ETriggerEvent::Started, this, &UDWInputManager::ReleaseSkillAbility4);
	
	InInputComponent->BindInputAction(GameplayTags::Input_UseInventoryItem, ETriggerEvent::Started, this, &UDWInputManager::UseInventoryItem);
	InInputComponent->BindInputAction(GameplayTags::Input_DiscardInventoryItem, ETriggerEvent::Started, this, &UDWInputManager::DiscardInventoryItem);
	InInputComponent->BindInputAction(GameplayTags::Input_PrevInventorySlot, ETriggerEvent::Started, this, &UDWInputManager::PrevInventorySlot);
	InInputComponent->BindInputAction(GameplayTags::Input_NextInventorySlot, ETriggerEvent::Started, this, &UDWInputManager::NextInventorySlot);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot1, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot1);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot2, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot2);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot3, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot3);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot4, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot4);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot5, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot5);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot6, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot6);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot7, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot7);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot8, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot8);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot9, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot9);
	InInputComponent->BindInputAction(GameplayTags::Input_SelectInventorySlot10, ETriggerEvent::Started, this, &UDWInputManager::SelectInventorySlot10);
	
	InInputComponent->BindInputAction(GameplayTags::Input_ZoomInMiniMap, ETriggerEvent::Started, this, &UDWInputManager::ZoomInMiniMap);
	InInputComponent->BindInputAction(GameplayTags::Input_ZoomOutMiniMap, ETriggerEvent::Started, this, &UDWInputManager::ZoomOutMiniMap);
	InInputComponent->BindInputAction(GameplayTags::Input_OpenMaxMapBox, ETriggerEvent::Started, this, &UDWInputManager::OpenMaxMapBox);
	InInputComponent->BindInputAction(GameplayTags::Input_OpenInventoryPanel, ETriggerEvent::Started, this, &UDWInputManager::OpenInventoryPanel);
	InInputComponent->BindInputAction(GameplayTags::Input_OpenGeneratePanel, ETriggerEvent::Started, this, &UDWInputManager::OpenGeneratePanel);
	InInputComponent->BindInputAction(GameplayTags::Input_OpenTaskPanel, ETriggerEvent::Started, this, &UDWInputManager::OpenTaskPanel);
	InInputComponent->BindInputAction(GameplayTags::Input_OpenContextInputBox, ETriggerEvent::Started, this, &UDWInputManager::OpenContextInputBox);
	InInputComponent->BindInputAction(GameplayTags::Input_OpenSwitcherPanel, ETriggerEvent::Started, this, &UDWInputManager::OpenSwitcherPanel);
}

void UDWInputManager::OnTurnCamera_Implementation(const FInputActionValue& InValue)
{
	Super::OnTurnCamera_Implementation(InValue);

	if(InValue.Get<float>() == 0.f) return;

	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive()) return;

	PlayerCharacter->Targeting->TargetActorWithAxisInput(InValue.Get<float>());
}

void UDWInputManager::OnTurnPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	IWHPlayerInterface::Execute_Turn(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::OnMoveForwardPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	IWHPlayerInterface::Execute_MoveForward(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::OnMoveRightPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	IWHPlayerInterface::Execute_MoveRight(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::OnMoveUpPlayer_Implementation(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	IWHPlayerInterface::Execute_MoveUp(PossessedCharacter, InValue.Get<float>());
}

void UDWInputManager::OnActionUpPlayer_Implementation(const FInputActionValue& InValue)
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	if(InValue.Get<float>() > 0.f)
	{
		if(PossessedCharacter->IsWalking() || PossessedCharacter->IsFloating())
		{
			PossessedCharacter->Jump();
		}
		else if(PossessedCharacter->IsFalling() && PossessedCharacter->GetMoveVelocity().Z < 0.f)
		{
			PossessedCharacter->Fly();
		}
		else if(PossessedCharacter->IsFlying())
		{
			// PossessedCharacter->UnFly();
		}
	}
}

void UDWInputManager::SystemOperation_Implementation()
{
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UDWProcedure_Playing>())
	{
		UProcedureModuleStatics::SwitchProcedureByClass<UDWProcedure_Pausing>();
	}
	else if(UProcedureModuleStatics::IsCurrentProcedureClass<UDWProcedure_Testing>())
	{
		UWidgetModuleStatics::OpenUserWidget<UWidgetSettingPanel>();
	}
}

void UDWInputManager::OnPrimaryPressed_Implementation()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	bPrimaryPressed = true;
	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			if(PlayerCharacter->IsFreeToAnim() || PlayerCharacter->IsAttacking())
			{
				AttackAbilityQueue++; 
			}
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult VoxelHitResult;
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, VoxelHitResult))
			{
				PlayerCharacter->InteractVoxel(EInputInteractAction::Primary, EInputInteractEvent::Started, VoxelHitResult);
			}
			break;
		}
	}
}

void UDWInputManager::OnPrimaryRepeated_Implementation()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult VoxelHitResult;
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, VoxelHitResult))
			{
				PlayerCharacter->InteractVoxel(EInputInteractAction::Primary, EInputInteractEvent::Triggered, VoxelHitResult);
			}
			break;
		}
	}
}

void UDWInputManager::OnPrimaryReleased_Implementation()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter) return;

	bPrimaryPressed = false;
	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult VoxelHitResult;
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, VoxelHitResult))
			{
				PlayerCharacter->InteractVoxel(EInputInteractAction::Primary, EInputInteractEvent::Completed, VoxelHitResult);
			}
			else
			{
				PlayerCharacter->UnInteractVoxel(EInputInteractAction::Primary, EInputInteractEvent::Completed);
			}
			break;
		}
	}
}

void UDWInputManager::OnSecondaryPressed_Implementation()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	bSecondaryPressed = true;
	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult VoxelHitResult;
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, VoxelHitResult))
			{
				PlayerCharacter->InteractVoxel(EInputInteractAction::Secondary, EInputInteractEvent::Started, VoxelHitResult);
			}
			break;
		}
	}
}

void UDWInputManager::OnSecondaryRepeated_Implementation()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult VoxelHitResult;
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, VoxelHitResult))
			{
				PlayerCharacter->InteractVoxel(EInputInteractAction::Secondary, EInputInteractEvent::Triggered, VoxelHitResult);
			}
			break;
		}
	}
}

void UDWInputManager::OnSecondaryReleased_Implementation()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter) return;

	bSecondaryPressed = false;
	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult VoxelHitResult;
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, VoxelHitResult))
			{
				PlayerCharacter->InteractVoxel(EInputInteractAction::Secondary, EInputInteractEvent::Completed, VoxelHitResult);
			}
			else
			{
				PlayerCharacter->UnInteractVoxel(EInputInteractAction::Secondary, EInputInteractEvent::Completed);
			}
			break;
		}
	}
}

void UDWInputManager::OnThirdPressed_Implementation()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	PlayerCharacter->GetTargeting()->TargetActor();

	bThirdPressed = true;
	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult VoxelHitResult;
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, VoxelHitResult))
			{
				PlayerCharacter->InteractVoxel(EInputInteractAction::Third, EInputInteractEvent::Started, VoxelHitResult);
			}
			break;
		}
	}
}

void UDWInputManager::OnThirdRepeated_Implementation()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult VoxelHitResult;
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, VoxelHitResult))
			{
				PlayerCharacter->InteractVoxel(EInputInteractAction::Third, EInputInteractEvent::Triggered, VoxelHitResult);
			}
			break;
		}
	}
}

void UDWInputManager::OnThirdReleased_Implementation()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter) return;

	bThirdPressed = false;
	switch (PlayerCharacter->ControlMode)
	{
		case EDWCharacterControlMode::Fighting:
		{
			break;
		}
		case EDWCharacterControlMode::Creating:
		{
			FVoxelHitResult VoxelHitResult;
			if(UVoxelModuleStatics::VoxelRaycastSinge(EVoxelRaycastType::FromAimPoint, PlayerCharacter->GetInteractDistance(), {}, VoxelHitResult))
			{
				PlayerCharacter->InteractVoxel(EInputInteractAction::Third, EInputInteractEvent::Completed, VoxelHitResult);
			}
			else
			{
				PlayerCharacter->UnInteractVoxel(EInputInteractAction::Third, EInputInteractEvent::Completed);
			}
			break;
		}
	}
}

void UDWInputManager::OnSprintPressed()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	bSprintPressed = true;
}

void UDWInputManager::OnSprintReleased()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter) return;
	
	bSprintPressed = false;
}

void UDWInputManager::OnDodgePressed()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	PossessedCharacter->Dodge();
}

void UDWInputManager::OnDodgeReleased()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	// PossessedCharacter->UnDodge();
}

void UDWInputManager::ToggleControlMode()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

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
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>(LocalPlayerIndex);

	if(!PossessedCharacter || !PossessedCharacter->IsActive()) return;

	if(!PossessedCharacter->IsCrouching())
	{
		PossessedCharacter->Crouch(true);
	}
	else
	{
		PossessedCharacter->UnCrouch(true);
	}
}

void UDWInputManager::ChangeHand()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);

	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	PlayerCharacter->ChangeHand();
}

void UDWInputManager::ReleaseSkillAbility1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	PlayerCharacter->Skill(0);
}

void UDWInputManager::ReleaseSkillAbility2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	PlayerCharacter->Skill(1);
}

void UDWInputManager::ReleaseSkillAbility3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	PlayerCharacter->Skill(2);
}

void UDWInputManager::ReleaseSkillAbility4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;

	PlayerCharacter->Skill(3);
}

void UDWInputManager::DoInteract1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(0))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[0]);
	}
}

void UDWInputManager::DoInteract2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(1))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[1]);
	}
}

void UDWInputManager::DoInteract3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(2))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[2]);
	}
}

void UDWInputManager::DoInteract4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(3))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[3]);
	}
}

void UDWInputManager::DoInteract5()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(4))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[4]);
	}
}

void UDWInputManager::NextInteract()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	if(PlayerCharacter->GetOverlappingAgents().Num() > 1)
	{
		PlayerCharacter->GetInteractingAgent()->LeaveInteract(PlayerCharacter);
	}
}

void UDWInputManager::UseInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(UInputModuleStatics::GetKeyShortcut(GameplayTags::Input_InventoryAll).IsPressing(UCommonStatics::GetPlayerController(LocalPlayerIndex)) ? -1 : 1);
}

void UDWInputManager::DiscardInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(UInputModuleStatics::GetKeyShortcut(GameplayTags::Input_InventoryAll).IsPressing(UCommonStatics::GetPlayerController(LocalPlayerIndex)) ? -1 : 1, false);
}

void UDWInputManager::PrevInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter) || UInputModuleStatics::GetKeyShortcut(GameplayTags::Input_CameraZoom).IsPressing(UCommonStatics::GetPlayerController(LocalPlayerIndex))) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->PrevInventorySlot();
}

void UDWInputManager::NextInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter) || UInputModuleStatics::GetKeyShortcut(GameplayTags::Input_CameraZoom).IsPressing(UCommonStatics::GetPlayerController(LocalPlayerIndex))) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->NextInventorySlot();
}

void UDWInputManager::SelectInventorySlot1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(0);
}

void UDWInputManager::SelectInventorySlot2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(1);
}

void UDWInputManager::SelectInventorySlot3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(2);
}

void UDWInputManager::SelectInventorySlot4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(3);
}

void UDWInputManager::SelectInventorySlot5()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(4);
}

void UDWInputManager::SelectInventorySlot6()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(5);
}

void UDWInputManager::SelectInventorySlot7()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(6);
}

void UDWInputManager::SelectInventorySlot8()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(7);
}

void UDWInputManager::SelectInventorySlot9()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(8);
}

void UDWInputManager::SelectInventorySlot10()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(9);
}

void UDWInputManager::ZoomInMiniMap()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter) return;
	
	USceneModuleStatics::SetMiniMapRange(USceneModuleStatics::GetMiniMapRange() + 1000.f);
}

void UDWInputManager::ZoomOutMiniMap()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter) return;
	
	USceneModuleStatics::SetMiniMapRange(USceneModuleStatics::GetMiniMapRange() - 1000.f);
}

void UDWInputManager::OpenMaxMapBox()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter) return;
	
	UWidgetModuleStatics::OpenUserWidget<UWidgetMaxMapBox>();
}

void UDWInputManager::OpenInventoryPanel()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::OpenUserWidget<UWidgetInventoryPanel>();
}

void UDWInputManager::OpenGeneratePanel()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
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
		const FVoxelItem& OverlappingVoxel = PlayerCharacter->GetCharacterPart(EDWCharacterPart::Chest)->GetOverlappingVoxel();
		if(OverlappingVoxel.IsValid() && OverlappingVoxel.GetVoxelType() == EVoxelType::Water)
		{
			UWidgetModuleStatics::OpenUserWidget<UWidgetGeneratePanel>({ &OverlappingVoxel.GetVoxel() });
		}
		else
		{
			UWidgetModuleStatics::OpenUserWidget<UWidgetGeneratePanel>();
		}
	}
}

void UDWInputManager::OpenTaskPanel()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::OpenUserWidget<UWidgetTaskPanel>();
}

void UDWInputManager::OpenContextInputBox()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::OpenUserWidget<UWidgetContextInputBox>();
}

void UDWInputManager::OpenSwitcherPanel()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>(LocalPlayerIndex);
	
	if(!PlayerCharacter || !PlayerCharacter->IsActive() || PlayerCharacter->Execute_IsBlockAllInput(PlayerCharacter)) return;
	
	UWidgetModuleStatics::OpenUserWidget<UWidgetSwitcherPanel>();
}
