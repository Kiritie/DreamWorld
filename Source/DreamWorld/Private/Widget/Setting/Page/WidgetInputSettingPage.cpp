// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetInputSettingPage.h"

#include "InputMappingContext.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Gameplay/DWGameMode.h"
#include "Input/DWInputModule.h"
#include "Input/Base/InputActionBase.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Item/WidgetKeySettingItem.h"

class UDWSettingSaveGame;

UWidgetInputSettingPage::UWidgetInputSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InputSettingPage");

	Title = FText::FromString(TEXT("输入"));
}

void UWidgetInputSettingPage::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetInputSettingPage::OnCreate(UObject* InOwner)
{
	Super::OnCreate(InOwner);

	for(auto Iter1 : ADWInputModule::Get()->GetActionContexts())
	{
		for(auto& Iter2 : Iter1->GetMappings())
		{
			const auto SettingItem = CreateSubWidget<UWidgetKeySettingItem>({ Iter2.Action->ActionDescription }, UAssetModuleBPLibrary::GetStaticClass(FName("KeySettingItem")));
			SettingItems.FindOrAdd(Cast<UInputActionBase>(Iter2.Action)->ActionName).Add(SettingItem);
			AddSettingItem(SettingItem, Iter1->ContextDescription);
		}
	}
}

void UWidgetInputSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	for(auto& Iter : SettingItems)
	{
		TArray<FEnhancedActionKeyMapping*> Mappings = ADWInputModule::Get()->GetActionMappingsByName(Iter.Key);
		for(int32 i = 0; i < Iter.Value.Num(); i++)
		{
			Iter.Value[i]->SetValue(Mappings[i]->Key.ToString());
		}
	}
}

void UWidgetInputSettingPage::OnApply()
{
	Super::OnApply();

	for(auto& Iter : SettingItems)
	{
		TArray<FEnhancedActionKeyMapping*> Mappings = ADWInputModule::Get()->GetActionMappingsByName(Iter.Key);
		for(int32 i = 0; i < Iter.Value.Num(); i++)
		{
			Mappings[i]->Key = FKey(*Iter.Value[i]->GetValue().GetStringValue());
		}
	}
}

void UWidgetInputSettingPage::OnReset()
{
	Super::OnReset();

	for(auto& Iter : SettingItems)
	{
		TArray<FEnhancedActionKeyMapping*> Mappings = GetDefaultInputData().GetActionMappingsByName(Iter.Key);
		for(int32 i = 0; i < Iter.Value.Num(); i++)
		{
			Iter.Value[i]->SetValue(Mappings[i]->Key.ToString());
		}
	}
}

void UWidgetInputSettingPage::OnValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue)
{
	Super::OnValueChange(InSettingItem, InValue);
}

void UWidgetInputSettingPage::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

bool UWidgetInputSettingPage::CanApply_Implementation() const
{
	for(auto& Iter : SettingItems)
	{
		TArray<FEnhancedActionKeyMapping*> Mappings = ADWInputModule::Get()->GetActionMappingsByName(Iter.Key);
		for(int32 i = 0; i < Iter.Value.Num(); i++)
		{
			if(Mappings[i]->Key.ToString() != Iter.Value[i]->GetValue().GetStringValue())
			{
				return true;
			}
		}
	}
	return false;
}

bool UWidgetInputSettingPage::CanReset_Implementation() const
{
	for(auto& Iter : SettingItems)
	{
		TArray<FEnhancedActionKeyMapping*> Mappings = GetDefaultInputData().GetActionMappingsByName(Iter.Key);
		for(int32 i = 0; i < Iter.Value.Num(); i++)
		{
			if(Mappings[i]->Key.ToString() != Iter.Value[i]->GetValue().GetStringValue())
			{
				return true;
			}
		}
	}
	return false;
}

FDWInputModuleSaveData& UWidgetInputSettingPage::GetDefaultInputData() const
{
	return USaveGameModuleBPLibrary::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().InputData;
}
