// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DWPlayerController.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Ability/Character/CharacterDataBase.h"
#include "Character/DWCharacterData.h"
#include "Widget/WidgetGameHUD.h"
#include "Gameplay/DWGameInstance.h"
#include "Gameplay/DWGameState.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "Inventory/Inventory.h"
#include "Gameplay/DWGameMode.h"
#include "Inventory/Slot/InventorySlot.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Voxel/VoxelModule.h"
#include "Widget/WidgetModuleBPLibrary.h"

ADWPlayerController::ADWPlayerController()
{
	bCameraControlAble = true;

	bCameraMoveAble = false;

	CameraRotateKey = FKey();

	CameraZoomKey = FKey("LeftCtrl");

	// camera
	CameraTurnRate = 45.f;
	CameraLookUpRate = 45.f;
	CameraRotateSmooth = 30.f;
	InitCameraPinch = -20.f;

	MinCameraDistance = 100.f;
	MaxCameraDistance = 300.f;
	InitCameraDistance = 150.f;

	// inputs
	bPressedSprint = false;
	DoubleJumpTime = 0.f;
}

void ADWPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ADWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->SetTickableWhenPaused(true);

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
	Super::OnPossess(InPawn);

	if(ADWPlayerCharacter* InCharacter = Cast<ADWPlayerCharacter>(InPawn))
	{
		if(InCharacter != PlayerCharacter)
		{
			PlayerCharacter = InCharacter;
			PlayerCharacter->SetOwnerController(this);
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGameHUD>(PlayerCharacter);
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryBar>(PlayerCharacter);
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryPanel>(PlayerCharacter);
			PlayerCharacter->RefreshData();
		}
	}
}

void ADWPlayerController::OnUnPossess()
{
	if(ADWPlayerCharacter* OwnerCharacter = Cast<ADWPlayerCharacter>(GetPawn()))
	{
		if(OwnerCharacter == PlayerCharacter && !PlayerCharacter->IsRiding())
		{
			PlayerCharacter->SetOwnerController(nullptr);
			PlayerCharacter = nullptr;
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetGameHUD>(nullptr);
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryBar>(nullptr);
			UWidgetModuleBPLibrary::InitializeUserWidget<UWidgetInventoryPanel>(nullptr);
		}
	}
	Super::OnUnPossess();
}

void ADWPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!GetProcessedCharacter() || GetProcessedCharacter()->IsDead()) return;

	if(GetProcessedCharacter()->IsActive())
	{
		if(bPressedSprint && GetProcessedCharacter()->GetMoveVelocity().Size() > 0.2f)
		{
			GetProcessedCharacter()->Sprint();
		}
		else
		{
			GetProcessedCharacter()->UnSprint();
		}

		if(DoubleJumpTime > 0.f)
		{
			DoubleJumpTime -= DeltaTime;
		}
	}
}

void ADWPlayerController::LoadData(FDWPlayerSaveData InPlayerData)
{
	if(GetProcessedCharacter()) return;

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if(ADWPlayerCharacter* NewPlayerCharacter = GetWorld()->SpawnActor<ADWPlayerCharacter>(InPlayerData.GetCharacterData().Class, SpawnParams))
	{
		SetControlRotation(InPlayerData.CameraRotation);
		if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
		{
			SetCameraDistance(GeneralSaveGame->SaveData.CameraDistance, true);
		}

		NewPlayerCharacter->Disable(true, true);
		NewPlayerCharacter->LoadData(&InPlayerData);

		Possess(NewPlayerCharacter);

		if(!InPlayerData.bSaved)
		{
			// auto VoxelDatas = UDWHelper::LoadVoxelDatas();
			// for (int32 i = 0; i < VoxelDatas.Num(); i++)
			// {
			// 	FAbilityItem tmpItem = FAbilityItem(VoxelDatas[i].ID, VoxelDatas[i].MaxCount);
			// 	NewPlayerCharacter->GetInventory()->AdditionItemByRange(tmpItem);
			// }
			//
			// auto EquipDatas = UDWHelper::LoadEquipDatas();
			// for (int32 i = 0; i < EquipDatas.Num(); i++)
			// {
			// 	FAbilityItem tmpItem = FAbilityItem(EquipDatas[i].ID, EquipDatas[i].MaxCount);
			// 	NewPlayerCharacter->GetInventory()->AdditionItemByRange(tmpItem);
			// }
			//
			// auto PropDatas = UDWHelper::LoadPropDatas();
			// for (int32 i = 0; i < PropDatas.Num(); i++)
			// {
			// 	FAbilityItem tmpItem = FAbilityItem(PropDatas[i].ID, PropDatas[i].MaxCount);
			// 	NewPlayerCharacter->GetInventory()->AdditionItemByRange(tmpItem);
			// }
			//
			// auto SkillDatas = UDWHelper::LoadSkillDatas();
			// for (int32 i = 0; i < SkillDatas.Num(); i++)
			// {
			// 	FAbilityItem tmpItem = FAbilityItem(SkillDatas[i].ID, SkillDatas[i].MaxCount);
			// 	NewPlayerCharacter->GetInventory()->AdditionItemByRange(tmpItem);
			// }
		}
	}
}

void ADWPlayerController::UnloadData()
{
	if(!GetProcessedCharacter()) return;

	UnPossess();
	GetProcessedCharacter()->Destroy();
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
		return UKismetSystemLibrary::LineTraceSingle(PlayerCharacter, rayStart, rayEnd, UDWHelper::GetGameTrace(InGameTraceType), false, TArray<AActor*>(), EDrawDebugTrace::None, OutHitResult, true);
	}
	return false;
}

void ADWPlayerController::OnJumpPressed()
{
	if(!GetProcessedCharacter() || GetProcessedCharacter()->IsBreakAllInput()) return;

	GetProcessedCharacter()->Jump();
	if(DoubleJumpTime <= 0.f)
	{
		DoubleJumpTime = 0.2f;
	}
	else
	{
		if(!GetProcessedCharacter()->IsFlying())
		{
			GetProcessedCharacter()->Fly();
		}
		else
		{
			GetProcessedCharacter()->UnFly();
		}
		DoubleJumpTime = 0.f;
	}
}

void ADWPlayerController::OnJumpReleased()
{
	GetProcessedCharacter()->UnJump();
}

void ADWPlayerController::OnSprintPressed()
{
	if(!GetProcessedCharacter() || GetProcessedCharacter()->IsBreakAllInput()) return;

	bPressedSprint = true;
}

void ADWPlayerController::OnSprintReleased()
{
	bPressedSprint = false;
}

void ADWPlayerController::ToggleInventoryPanel()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::ToggleUserWidget<UWidgetInventoryPanel>(false);
		}
	}
}

void ADWPlayerController::UseInventoryItem()
{
	if(!GetProcessedCharacter() || GetProcessedCharacter()->IsBreakAllInput()) return;

	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(1);
		}
	}
}

void ADWPlayerController::UseAllInventoryItem()
{
	if(!GetProcessedCharacter() || GetProcessedCharacter()->IsBreakAllInput()) return;

	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->UseItem(-1);
		}
	}
}

void ADWPlayerController::DiscardInventoryItem()
{
	if(!GetProcessedCharacter() || GetProcessedCharacter()->IsBreakAllInput()) return;

	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(1);
		}
	}
}

void ADWPlayerController::DiscardAllInventoryItem()
{
	if(!GetProcessedCharacter() || GetProcessedCharacter()->IsBreakAllInput()) return;

	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->GetSelectedSlot()->DiscardItem(-1);
		}
	}
}

void ADWPlayerController::PrevInventorySlot()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->PrevInventorySlot();
		}
	}
}

void ADWPlayerController::NextInventorySlot()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->NextInventorySlot();
		}
	}
}

void ADWPlayerController::SelectInventorySlot1()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(0);
		}
	}
}

void ADWPlayerController::SelectInventorySlot2()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(1);
		}
	}
}

void ADWPlayerController::SelectInventorySlot3()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(2);
		}
	}
}

void ADWPlayerController::SelectInventorySlot4()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(3);
		}
	}
}

void ADWPlayerController::SelectInventorySlot5()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(4);
		}
	}
}

void ADWPlayerController::SelectInventorySlot6()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(5);
		}
	}
}

void ADWPlayerController::SelectInventorySlot7()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(6);
		}
	}
}

void ADWPlayerController::SelectInventorySlot8()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(7);
		}
	}
}

void ADWPlayerController::SelectInventorySlot9()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(8);
		}
	}
}

void ADWPlayerController::SelectInventorySlot10()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(GameState->GetCurrentState() == EDWGameState::Playing)
		{
			UWidgetModuleBPLibrary::GetUserWidget<UWidgetInventoryBar>()->SelectInventorySlot(9);
		}
	}
}

void ADWPlayerController::PauseOrContinueGame()
{
	if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
	{
		if(ADWGameMode* GameMode = UGlobalBPLibrary::GetGameMode<ADWGameMode>(this))
		{
			if(GameState->GetCurrentState() == EDWGameState::Playing)
			{
				GameMode->PauseGame();
			}
			else if(GameState->GetCurrentState() == EDWGameState::Pausing)
			{
				GameMode->UnPauseGame();
			}
		}
	}
}

ADWCharacter* ADWPlayerController::GetProcessedCharacter() const
{
	return Cast<ADWCharacter>(GetPawn());
}
