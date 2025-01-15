// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Context/WidgetContextInputBox.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"
#include "Components/EditableTextBox.h"
#include "Widget/WidgetModule.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Context/WidgetContextBox.h"
#include "Ability/Inventory/AbilityInventoryBase.h"

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
		if(!InText.IsEmpty() && InCommitMethod != ETextCommit::OnCleared)
		{
			FString Message = InText.ToString();
			if(Message.StartsWith(TEXT("/")) && Message.Len() > 1)
			{
				FString Command = Message.Mid(1, Message.Len());
				FString Params;
				Command.Split(TEXT(" "), &Command, &Params);

				Message = TEXT("");
				if(Command.Equals(TEXT("AddItem"), ESearchCase::IgnoreCase))
				{
					TArray<FString> ParamArr;
					if(Params.ParseIntoArray(ParamArr, TEXT(" ")) >= 1)
					{
						FAbilityItem Item = FAbilityItem(FPrimaryAssetId(*ParamArr[0]), ParamArr.IsValidIndex(1) ? FCString::Atoi(*ParamArr[1]) : 1, ParamArr.IsValidIndex(2) ? FCString::Atoi(*ParamArr[2]) : 0);
						ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
						if(PlayerCharacter && PlayerCharacter->IsActive())
						{
							PlayerCharacter->GetInventory()->AddItemByRange(Item, -1);
						}
					}
					else
					{
						Message = FString::Printf(TEXT("命令参数错误: %s"), *Command);
					}
				}
				else if(Command.Equals(TEXT("Kill"), ESearchCase::IgnoreCase))
				{
					ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
					if(PlayerCharacter && PlayerCharacter->IsActive())
					{
						PlayerCharacter->Death(PlayerCharacter);
					}
				}
				else if(Command.Equals(TEXT("LevelUp"), ESearchCase::IgnoreCase))
				{
					ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();
					if(PlayerCharacter && PlayerCharacter->IsActive())
					{
						PlayerCharacter->SetLevelA(PlayerCharacter->GetLevelA() + 1);
					}
				}
				else
				{
					Message = FString::Printf(TEXT("无效命令: %s"), *Command);
				}
			}
			if(!Message.IsEmpty())
			{
				UWidgetModuleStatics::GetUserWidget<UWidgetContextBox>()->AddMessage(Message);
			}
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
