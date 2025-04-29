// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Main/WidgetGameHUD.h"

#include "Character/DWCharacter.h"
#include "Event/EventModuleStatics.h"
#include "Event/Handle/Input/EventHandle_InputModeChanged.h"
#include "Input/InputModuleStatics.h"
#include "Widget/WidgetModule.h"


UWidgetGameHUD::UWidgetGameHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GameHUD");
	WidgetType = EWidgetType::Permanent;
	WidgetInputMode = EInputMode::GameOnly;
}

void UWidgetGameHUD::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	UEventModuleStatics::SubscribeEvent<UEventHandle_InputModeChanged>(this, GET_FUNCTION_NAME_THISCLASS(OnInputModeChanged));
}

void UWidgetGameHUD::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetGameHUD::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetGameHUD::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

void UWidgetGameHUD::OnRefresh()
{
	Super::OnRefresh();
}

FReply UWidgetGameHUD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(UInputModuleStatics::IsPlayerMappedKeyByTag(GameplayTags::Input_SystemOperation, InKeyEvent.GetKey()))
	{
		if(TemporaryChild)
		{
			TemporaryChild->Close();
			return FReply::Handled();
		}
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UWidgetGameHUD::OnInputModeChanged(UObject* InSender, UEventHandle_InputModeChanged* InEventHandle)
{
	if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(OwnerObject))
	{
		if(!OwnerCharacter->IsDead() && InEventHandle->InputMode == EInputMode::GameOnly)
		{
			SetCrosshairVisible(true);
		}
		else
		{
			SetCrosshairVisible(false);
		}
	}
	else
	{
		SetCrosshairVisible(true);
	}
}
