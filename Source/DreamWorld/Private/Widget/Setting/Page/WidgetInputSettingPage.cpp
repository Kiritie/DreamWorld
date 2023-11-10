// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetInputSettingPage.h"

#include "InputMappingContext.h"
#include "Asset/AssetModuleStatics.h"
#include "Gameplay/DWGameMode.h"
#include "Input/DWInputModule.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Widget/WidgetModuleStatics.h"
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

	for (const auto& Mapping : UInputModule::Get().GetAllPlayerMappableActionKeyMappings())
	{
		if (Mapping.PlayerMappableOptions.Name != NAME_None && !Mapping.PlayerMappableOptions.DisplayName.IsEmpty())
		{
			UWidgetSettingItemBase* SettingItem = nullptr;
			for(auto Iter : SettingItems)
			{
				if(Iter->GetLabel().EqualTo(Mapping.PlayerMappableOptions.DisplayName))
				{
					SettingItem = Iter;
					break;
				}
			}
			if(!SettingItem)
			{
				SettingItem = CreateSubWidget<UWidgetKeySettingItem>({ Mapping.PlayerMappableOptions.DisplayName }, UAssetModuleStatics::GetStaticClass(FName("KeySettingItem")));
			}
			AddSettingItem(SettingItem, Mapping.PlayerMappableOptions.DisplayCategory);
		}
	}
}

void UWidgetInputSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	for(auto& Iter1 : SettingItems)
	{
		TArray<FEnhancedActionKeyMapping> Mappings = UDWInputModule::Get().GetAllActionMappingByDisplayName(Iter1->GetLabel());
		TArray<FParameter> Values;
		for(auto& Iter2 : Mappings)
		{
			Values.Add(Iter2.Key.ToString());
		}
		Iter1->SetValues(Values);
	}
}

void UWidgetInputSettingPage::OnApply()
{
	Super::OnApply();

	for(auto& Iter : SettingItems)
	{
		TArray<FEnhancedActionKeyMapping> Mappings = UDWInputModule::Get().GetAllActionMappingByDisplayName(Iter->GetLabel());
		TArray<FParameter> Values = Iter->GetValues();
		for(int32 i = 0; i < Values.Num(); i++)
		{
			if(*Values[i].GetStringValue() != Mappings[i].Key)
			{
				UInputModule::Get().AddOrUpdateCustomKeyboardBindings(Mappings[i].PlayerMappableOptions.Name, *Values[i].GetStringValue(), GetOwningLocalPlayer()->GetLocalPlayerIndex());
			}
		}
	}
}

void UWidgetInputSettingPage::OnReset()
{
	Super::OnReset();

	for(auto& Iter : SettingItems)
	{
		TArray<FEnhancedActionKeyMapping> Mappings = GetDefaultInputData().GetAllActionMappingByDisplayName(Iter->GetLabel());
		TArray<FParameter> Values = Iter->GetValues();
		for(int32 i = 0; i < Values.Num(); i++)
		{
			if(*Values[i].GetStringValue() != Mappings[i].Key)
			{
				Values[i] = Mappings[i].Key.ToString();
			}
			Iter->SetValues(Values);
		}
	}
}

void UWidgetInputSettingPage::OnValuesChange(UWidgetSettingItemBase* InSettingItem, const TArray<FParameter>& InValues)
{
	Super::OnValuesChange(InSettingItem, InValues);
}

void UWidgetInputSettingPage::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

bool UWidgetInputSettingPage::CanApply_Implementation() const
{
	for(auto& Iter : SettingItems)
	{
		TArray<FEnhancedActionKeyMapping> Mappings = UDWInputModule::Get().GetAllActionMappingByDisplayName(Iter->GetLabel());
		TArray<FParameter> Values = Iter->GetValues();
		for(int32 i = 0; i < Values.Num(); i++)
		{
			if(*Values[i].GetStringValue() != Mappings[i].Key)
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
		TArray<FEnhancedActionKeyMapping> Mappings1 = UDWInputModule::Get().GetAllActionMappingByDisplayName(Iter->GetLabel());
		TArray<FEnhancedActionKeyMapping> Mappings2 = GetDefaultInputData().GetAllActionMappingByDisplayName(Iter->GetLabel());
		for(int32 i = 0; i < Mappings1.Num(); i++)
		{
			if(Mappings1[i].Key != Mappings2[i].Key)
			{
				return true;
			}
		}
	}
	return false;
}

FDWInputModuleSaveData& UWidgetInputSettingPage::GetDefaultInputData() const
{
	return USaveGameModuleStatics::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().InputData;
}
