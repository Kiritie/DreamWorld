// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Switcher/WidgetSwitcherPanel.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Switcher/WidgetSwitcherBox.h"

UWidgetSwitcherPanel::UWidgetSwitcherPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("SwitcherPanel");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::UIOnly;
	WidgetType = EWidgetType::Temporary;

	SetIsFocusable(true);
}

void UWidgetSwitcherPanel::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	Switcher_ControlMode->OnButtonSelected.AddDynamic(this, &UWidgetSwitcherPanel::OnControlModeButtonSelected);
	Switcher_WeaponGroup->OnButtonSelected.AddDynamic(this, &UWidgetSwitcherPanel::OnWeaponGroupButtonSelected);
}

void UWidgetSwitcherPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetSwitcherPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();

	Super::OnOpen(InParams, bInstant);

	ADWPlayerCharacter* Character = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!Character) return;
	
	Switcher_ControlMode->SetSelectedButtonIndex((int32)Character->GetControlMode());
	Switcher_WeaponGroup->SetSelectedButtonIndex((int32)Character->GetWeaponGroup());
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

void UWidgetSwitcherPanel::OnControlModeButtonSelected(int32 ButtonIndex)
{
	ADWPlayerCharacter* Character = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!Character || !IsWidgetOpened(false)) return;
	
	Character->SetControlMode((EDWCharacterControlMode)ButtonIndex);

	Close();
}

void UWidgetSwitcherPanel::OnWeaponGroupButtonSelected(int32 ButtonIndex)
{
	ADWPlayerCharacter* Character = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!Character || !IsWidgetOpened(false)) return;
	
	Character->SetWeaponGroup((EDWWeaponGroup)ButtonIndex);

	Close();
}
