// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Switcher/WidgetSwitcherPanel.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Input/InputModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Switcher/WidgetSwitcherBox.h"

UWidgetSwitcherPanel::UWidgetSwitcherPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("SwitcherPanel");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::UIOnly;
	WidgetType = EWidgetType::Temporary;

	bWidgetAutoFocus = true;
	
	SetIsFocusable(true);
}

void UWidgetSwitcherPanel::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	Switcher_ControlMode->OnButtonSelected.AddDynamic(this, &UWidgetSwitcherPanel::OnControlModeButtonSelected);
	Switcher_WeaponGroup->OnButtonSelected.AddDynamic(this, &UWidgetSwitcherPanel::OnWeaponGroupButtonSelected);
	Switcher_GenerateTool->OnButtonSelected.AddDynamic(this, &UWidgetSwitcherPanel::OnGenerateToolButtonSelected);
}

void UWidgetSwitcherPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetSwitcherPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
	
	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();

	ADWPlayerCharacter* Character = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!Character) return;
	
	Switcher_ControlMode->SetSelectedButtonIndex((int32)Character->GetControlMode());
	Switcher_WeaponGroup->SetSelectedButtonIndex((int32)Character->GetWeaponGroup());
	Switcher_GenerateTool->SetSelectedButtonIndex((int32)Character->GetGenerateToolType() - 1);
}

void UWidgetSwitcherPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();
}

void UWidgetSwitcherPanel::OnRefresh()
{
	Super::OnRefresh();
}

FReply UWidgetSwitcherPanel::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(UInputModuleStatics::IsPlayerMappedKeyByTag(GameplayTags::Input_OpenSwitcherPanel, InKeyEvent.GetKey()))
	{
		Close();
		return FReply::Handled();
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UWidgetSwitcherPanel::OnControlModeButtonSelected(int32 ButtonIndex)
{
	Switcher_WeaponGroup->SetVisibility(ButtonIndex == 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	Switcher_GenerateTool->SetVisibility(ButtonIndex == 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	ADWPlayerCharacter* Character = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!Character || !IsWidgetOpened(false)) return;
	
	Character->SetControlMode((EDWCharacterControlMode)ButtonIndex);
}

void UWidgetSwitcherPanel::OnWeaponGroupButtonSelected(int32 ButtonIndex)
{
	ADWPlayerCharacter* Character = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!Character || !IsWidgetOpened(false)) return;
	
	Character->SetWeaponGroup((EDWWeaponGroup)ButtonIndex);
}

void UWidgetSwitcherPanel::OnGenerateToolButtonSelected(int32 ButtonIndex)
{
	ADWPlayerCharacter* Character = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!Character || !IsWidgetOpened(false)) return;
	
	Character->SetGenerateToolType((EVoxelGenerateToolType)(ButtonIndex + 1));
}
