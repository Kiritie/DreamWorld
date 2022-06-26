// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DWPlayerController.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Camera/CameraModuleBPLibrary.h"
#include "Character/DWCharacterData.h"
#include "Widget/WidgetGameHUD.h"
#include "Gameplay/DWGameInstance.h"
#include "Gameplay/DWGameState.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Inventory/Inventory.h"
#include "Gameplay/DWGameMode.h"
#include "Global/GlobalBPLibrary.h"
#include "Input/InputModuleBPLibrary.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_Pausing.h"
#include "Procedure/Procedure_Playing.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Datas/VoxelData.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"

ADWPlayerController::ADWPlayerController()
{
	// inputs
	bPressedSprint = false;
	DoubleJumpTime = 0.f;
}

void ADWPlayerController::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWPlayerController::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
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
	if(InPawn->IsA(ADWPlayerCharacter::StaticClass()) && (InPawn != GetPlayerPawn() || !GetPawn()))
	{
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGameHUD>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryBar>(InPawn);
		UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryPanel>(InPawn);
	}
	Super::OnPossess(InPawn);

	UCameraModuleBPLibrary::SetCameraDistance(USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>()->GetCameraDistance(), true);
}

void ADWPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ADWPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!GetPawn<ADWCharacter>() || GetPawn<ADWCharacter>()->IsDead()) return;

	if(GetPawn<ADWCharacter>()->IsActive())
	{
		if(bPressedSprint && GetPawn<ADWCharacter>()->GetMoveVelocity().Size() > 0.2f)
		{
			GetPawn<ADWCharacter>()->Sprint();
		}
		else
		{
			GetPawn<ADWCharacter>()->UnSprint();
		}

		if(DoubleJumpTime > 0.f)
		{
			DoubleJumpTime -= DeltaTime;
		}
	}
}

void ADWPlayerController::LoadData(FSaveData* InSaveData)
{
	if(GetPlayerPawn<ADWPlayerCharacter>()) return;

	FDWPlayerSaveData InPlayerData = *static_cast<FDWPlayerSaveData*>(InSaveData);

	if(ADWPlayerCharacter* PlayerCharacter = UObjectPoolModuleBPLibrary::SpawnObject<ADWPlayerCharacter>(nullptr, InPlayerData.GetCharacterData().Class))
	{
		SetPlayerPawn(PlayerCharacter);

		PlayerCharacter->Disable(true, true);
		PlayerCharacter->LoadData(&InPlayerData);
	}
}

FSaveData* ADWPlayerController::ToData()
{
	return nullptr;
}

void ADWPlayerController::UnloadData()
{
	if(!GetPlayerPawn<ADWPlayerCharacter>()) return;

	UnPossess();
	GetPlayerPawn<ADWPlayerCharacter>()->Destroy();
	SetPlayerPawn(nullptr);
}

void ADWPlayerController::ResetData()
{
	bPressedSprint = false;
	DoubleJumpTime = 0.f;
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

void ADWPlayerController::OnJumpPressed(FKey Key)
{
	if(!GetPawn<ADWCharacter>() || GetPawn<ADWCharacter>()->IsBreakAllInput()) return;

	GetPawn<ADWCharacter>()->Jump();
	if(DoubleJumpTime <= 0.f)
	{
		DoubleJumpTime = 0.2f;
	}
	else
	{
		if(!GetPawn<ADWCharacter>()->IsFlying())
		{
			GetPawn<ADWCharacter>()->Fly();
		}
		else
		{
			GetPawn<ADWCharacter>()->UnFly();
		}
		DoubleJumpTime = 0.f;
	}
}

void ADWPlayerController::OnJumpReleased()
{
	GetPawn<ADWCharacter>()->UnJump();
}

void ADWPlayerController::OnSprintPressed()
{
	if(!GetPawn<ADWCharacter>() || GetPawn<ADWCharacter>()->IsBreakAllInput()) return;

	bPressedSprint = true;
}

void ADWPlayerController::OnSprintReleased()
{
	bPressedSprint = false;
}

void ADWPlayerController::ToggleInventoryPanel()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::ToggleUserWidget<UWidgetInventoryPanel>(false);
	}
}

void ADWPlayerController::UseInventoryItem()
{
	if(!GetPawn<ADWCharacter>() || GetPawn<ADWCharacter>()->IsBreakAllInput()) return;

	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(1);
	}
}

void ADWPlayerController::UseAllInventoryItem()
{
	if(!GetPawn<ADWCharacter>() || GetPawn<ADWCharacter>()->IsBreakAllInput()) return;

	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(-1);
	}
}

void ADWPlayerController::DiscardInventoryItem()
{
	if(!GetPawn<ADWCharacter>() || GetPawn<ADWCharacter>()->IsBreakAllInput()) return;

	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(1);
	}
}

void ADWPlayerController::DiscardAllInventoryItem()
{
	if(!GetPawn<ADWCharacter>() || GetPawn<ADWCharacter>()->IsBreakAllInput()) return;

	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(-1);
	}
}

void ADWPlayerController::PrevInventorySlot()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->PrevInventorySlot();
	}
}

void ADWPlayerController::NextInventorySlot()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->NextInventorySlot();
	}
}

void ADWPlayerController::SelectInventorySlot1()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(0);
	}
}

void ADWPlayerController::SelectInventorySlot2()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(1);
	}
}

void ADWPlayerController::SelectInventorySlot3()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(2);
	}
}

void ADWPlayerController::SelectInventorySlot4()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(3);
	}
}

void ADWPlayerController::SelectInventorySlot5()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(4);
	}
}

void ADWPlayerController::SelectInventorySlot6()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(5);
	}
}

void ADWPlayerController::SelectInventorySlot7()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(6);
	}
}

void ADWPlayerController::SelectInventorySlot8()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(7);
	}
}

void ADWPlayerController::SelectInventorySlot9()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(8);
	}
}

void ADWPlayerController::SelectInventorySlot10()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(9);
	}
}

void ADWPlayerController::PauseOrContinueGame()
{
	if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Playing)
	{
		UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Pausing>();
	}
	else if(UGlobalBPLibrary::GetGameState<ADWGameState>()->GetCurrentState() == EDWGameState::Pausing)
	{
		UProcedureModuleBPLibrary::SwitchProcedureByClass<UProcedure_Playing>();
	}
}
