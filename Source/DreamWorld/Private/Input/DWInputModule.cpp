// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DWInputModule.h"

#include "PlayerMappableInputConfig.h"
#include "Ability/Character/AbilityCharacterInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Camera/CameraModule.h"
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

IMPLEMENTATION_MODULE(UDWInputModule)

// ParamSets default values
UDWInputModule::UDWInputModule()
{
	bPressedAttackDestroy = false;
	bPressedDefendGenerate = false;
	bPressedSprint = false;
}

UDWInputModule::~UDWInputModule()
{
	TERMINATION_MODULE(UDWInputModule)
}

#if WITH_EDITOR
void UDWInputModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWInputModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWInputModule)
}
#endif

void UDWInputModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWInputModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWInputModule::OnRefresh(float DeltaSeconds)
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

void UDWInputModule::OnReset()
{
	Super::OnReset();
}

void UDWInputModule::OnPause()
{
	Super::OnPause();

	bPressedAttackDestroy = false;
	bPressedDefendGenerate = false;
	bPressedSprint = false;
}

void UDWInputModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWInputModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	const auto& SaveData = InSaveData->CastRef<FDWInputModuleSaveData>();
}

void UDWInputModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FSaveData* UDWInputModule::ToData()
{
	static FDWInputModuleSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FInputModuleSaveData>();

	return &SaveData;
}

void UDWInputModule::OnBindAction_Implementation(UInputComponentBase* InInputComponent, UPlayerMappableInputConfig* InInputConfig)
{
	Super::OnBindAction_Implementation(InInputComponent, InInputConfig);

	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Sprint, ETriggerEvent::Started, this, &UDWInputModule::OnSprintPressed);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Sprint, ETriggerEvent::Completed, this, &UDWInputModule::OnSprintReleased);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Interact1, ETriggerEvent::Started, this, &UDWInputModule::DoInteractAction1);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Interact2, ETriggerEvent::Started, this, &UDWInputModule::DoInteractAction2);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Interact3, ETriggerEvent::Started, this, &UDWInputModule::DoInteractAction3);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Interact4, ETriggerEvent::Started, this, &UDWInputModule::DoInteractAction4);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Interact5, ETriggerEvent::Started, this, &UDWInputModule::DoInteractAction5);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Dodge, ETriggerEvent::Started, this, &UDWInputModule::OnDodgePressed);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Dodge, ETriggerEvent::Completed, this, &UDWInputModule::OnDodgeReleased);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_ToggleCrouch, ETriggerEvent::Started, this, &UDWInputModule::ToggleCrouch);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_ToggleControlMode, ETriggerEvent::Started, this, &UDWInputModule::ToggleControlMode);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_ToggleLockSightTarget, ETriggerEvent::Started, this, &UDWInputModule::ToggleLockTarget);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_ChangeHand, ETriggerEvent::Started, this, &UDWInputModule::ChangeHand);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Primary, ETriggerEvent::Started, this, &UDWInputModule::OnAttackDestroyPressed);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Primary, ETriggerEvent::Completed, this, &UDWInputModule::OnAttackDestroyReleased);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Minor, ETriggerEvent::Started, this, &UDWInputModule::OnDefendGeneratePressed);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_Minor, ETriggerEvent::Completed, this, &UDWInputModule::OnDefendGenerateReleased);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_ReleaseSkillAbility1, ETriggerEvent::Started, this, &UDWInputModule::ReleaseSkillAbility1);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_ReleaseSkillAbility2, ETriggerEvent::Started, this, &UDWInputModule::ReleaseSkillAbility2);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_ReleaseSkillAbility3, ETriggerEvent::Started, this, &UDWInputModule::ReleaseSkillAbility3);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_ReleaseSkillAbility4, ETriggerEvent::Started, this, &UDWInputModule::ReleaseSkillAbility4);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_UseInventoryItem, ETriggerEvent::Started, this, &UDWInputModule::UseInventoryItem);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_DiscardInventoryItem, ETriggerEvent::Started, this, &UDWInputModule::DiscardInventoryItem);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_PrevInventorySlot, ETriggerEvent::Started, this, &UDWInputModule::PrevInventorySlot);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_NextInventorySlot, ETriggerEvent::Started, this, &UDWInputModule::NextInventorySlot);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot1, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot1);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot2, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot2);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot3, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot3);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot4, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot4);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot5, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot5);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot6, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot6);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot7, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot7);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot8, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot8);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot9, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot9);
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_SelectInventorySlot10, ETriggerEvent::Started, this, &UDWInputModule::SelectInventorySlot10);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_OpenInventoryPanel, ETriggerEvent::Started, this, &UDWInputModule::OpenInventoryPanel);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_OpenGeneratePanel, ETriggerEvent::Started, this, &UDWInputModule::OpenGeneratePanel);
	
	InInputComponent->BindInputAction(InInputConfig, GameplayTags::InputTag_PauseGame, ETriggerEvent::Started, this, &UDWInputModule::PauseGame);
}

void UDWInputModule::TurnPlayer(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_Turn(PossessedCharacter, InValue.Get<float>());
}

void UDWInputModule::MoveHPlayer(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_MoveH(PossessedCharacter, InValue.Get<float>());
}

void UDWInputModule::MoveVPlayer(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_MoveV(PossessedCharacter, InValue.Get<float>());
}

void UDWInputModule::MoveForwardPlayer(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_MoveForward(PossessedCharacter, InValue.Get<float>());
}

void UDWInputModule::MoveRightPlayer(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	IWHPlayerInterface::Execute_MoveRight(PossessedCharacter, InValue.Get<float>());
}

void UDWInputModule::MoveUpPlayer(const FInputActionValue& InValue)
{
	if(InValue.Get<float>() == 0.f) return;
	
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	if(PossessedCharacter->IsWalking())
	{
		if(InValue.Get<float>() > 0.f)
		{
			PossessedCharacter->Jump();
		}
	}
	else
	{
		IWHPlayerInterface::Execute_MoveUp(PossessedCharacter, InValue.Get<float>());
	}
}

void UDWInputModule::OnSprintPressed()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	bPressedSprint = true;
}

void UDWInputModule::OnSprintReleased()
{
	bPressedSprint = false;
}

void UDWInputModule::ToggleControlMode()
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

void UDWInputModule::ToggleCrouch()
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

void UDWInputModule::ToggleLockTarget()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	PlayerCharacter->GetTargeting()->TargetActor();
}

void UDWInputModule::ChangeHand()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	PlayerCharacter->ChangeHand();
}

void UDWInputModule::OnDodgePressed()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	PossessedCharacter->Dodge();
}

void UDWInputModule::OnDodgeReleased()
{
	ADWCharacter* PossessedCharacter = UCommonStatics::GetPossessedPawn<ADWCharacter>();

	if(!PossessedCharacter || PossessedCharacter->IsBreakAllInput()) return;

	// PossessedCharacter->UnDodge();
}

void UDWInputModule::OnAttackDestroyPressed()
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
				PlayerCharacter->OnInteractVoxel(voxelHitResult, EInputInteractAction::Action1);
			}
			break;
		}
	}
}

void UDWInputModule::OnAttackDestroyReleased()
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

void UDWInputModule::OnDefendGeneratePressed()
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
				PlayerCharacter->OnInteractVoxel(voxelHitResult, EInputInteractAction::Action2);
			}
			break;
		}
	}
}

void UDWInputModule::OnDefendGenerateReleased()
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

void UDWInputModule::ReleaseSkillAbility1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill).IsValidIndex(0))
	{
		PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill)[0]->ActiveItem();
	}
}

void UDWInputModule::ReleaseSkillAbility2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill).IsValidIndex(1))
	{
		PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill)[1]->ActiveItem();
	}
}

void UDWInputModule::ReleaseSkillAbility3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill).IsValidIndex(2))
	{
		PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill)[2]->ActiveItem();
	}
}

void UDWInputModule::ReleaseSkillAbility4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;

	if(PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill).IsValidIndex(3))
	{
		PlayerCharacter->Inventory->GetSlotsBySplitType(ESlotSplitType::Skill)[3]->ActiveItem();
	}
}

void UDWInputModule::DoInteractAction1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(0))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[0]);
	}
}

void UDWInputModule::DoInteractAction2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(1))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[1]);
	}
}

void UDWInputModule::DoInteractAction3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(2))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[2]);
	}
}

void UDWInputModule::DoInteractAction4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(3))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[3]);
	}
}

void UDWInputModule::DoInteractAction5()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || !PlayerCharacter->GetInteractingAgent()) return;
	
	if(PlayerCharacter->GetInteractableActions().IsValidIndex(4))
	{
		PlayerCharacter->DoInteract(PlayerCharacter->GetInteractableActions()[4]);
	}
}

void UDWInputModule::OpenInventoryPanel()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::OpenUserWidget<UWidgetInventoryPanel>(false);
	}
}

void UDWInputModule::OpenGeneratePanel()
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

void UDWInputModule::UseInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(UInputModuleStatics::GetKeyShortcutByName("InventoryAll").IsPressing(GetPlayerController()) ? -1 : 1);
	}
}

void UDWInputModule::DiscardInventoryItem()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(UInputModuleStatics::GetKeyShortcutByName("InventoryAll").IsPressing(GetPlayerController()) ? -1 : 1, false);
	}
}

void UDWInputModule::PrevInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || UInputModuleStatics::GetKeyShortcutByName(FName("CameraZoom")).IsPressing(GetPlayerController())) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->PrevInventorySlot();
	}
}

void UDWInputModule::NextInventorySlot()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput() || UInputModuleStatics::GetKeyShortcutByName(FName("CameraZoom")).IsPressing(GetPlayerController())) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->NextInventorySlot();
	}
}

void UDWInputModule::SelectInventorySlot1()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(0);
	}
}

void UDWInputModule::SelectInventorySlot2()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(1);
	}
}

void UDWInputModule::SelectInventorySlot3()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(2);
	}
}

void UDWInputModule::SelectInventorySlot4()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(3);
	}
}

void UDWInputModule::SelectInventorySlot5()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(4);
	}
}

void UDWInputModule::SelectInventorySlot6()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(5);
	}
}

void UDWInputModule::SelectInventorySlot7()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(6);
	}
}

void UDWInputModule::SelectInventorySlot8()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(7);
	}
}

void UDWInputModule::SelectInventorySlot9()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(8);
	}
}

void UDWInputModule::SelectInventorySlot10()
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
	
	if(!PlayerCharacter || PlayerCharacter->IsBreakAllInput()) return;
	
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(9);
	}
}

void UDWInputModule::PauseGame()
{
	if(UProcedureModuleStatics::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		UProcedureModuleStatics::SwitchProcedureByClass<UProcedure_Pausing>();
	}
}
