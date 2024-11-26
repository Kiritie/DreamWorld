// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Context/WidgetContextInputBox.h"

#include "Components/EditableTextBox.h"
#include "Widget/WidgetModule.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Context/WidgetContextBox.h"

UWidgetContextInputBox::UWidgetContextInputBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("ContextInputBox");
	ParentName = FName("ContextBox");
	ParentSlot = FName("Slot_ContextInputBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreate;
	WidgetInputMode = EInputMode::UIOnly;
	
	TxtBox = nullptr;
	BtnSend = nullptr;
}

void UWidgetContextInputBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	TxtBox->OnTextCommitted.AddDynamic(this, &UWidgetContextInputBox::OnTextBoxValueCommitted);

	BtnSend->OnClicked().AddUObject(this, &UWidgetContextInputBox::OnSendButtonClicked);
}

void UWidgetContextInputBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetContextInputBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	TxtBox->SetText(FText::GetEmpty());
	TxtBox->SetFocus();

	FSimpleDynamicDelegate Delegate;
	Delegate.BindDynamic(this, &UWidgetContextInputBox::OnUIMaskPressed);
	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>({ Delegate });

	UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->ShowBorder();
}

void UWidgetContextInputBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();

	UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->HideBorder();
}

void UWidgetContextInputBox::OnTextBoxValueCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if(InCommitMethod == ETextCommit::OnEnter || InCommitMethod == ETextCommit::OnCleared)
	{
		if(!InText.IsEmpty())
		{
			UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(InText.ToString());
		}
		Close();
	}
}

void UWidgetContextInputBox::OnSendButtonClicked()
{
	OnTextBoxValueCommitted(TxtBox->GetText(), ETextCommit::OnEnter);
}

void UWidgetContextInputBox::OnUIMaskPressed()
{
	Close();
}
