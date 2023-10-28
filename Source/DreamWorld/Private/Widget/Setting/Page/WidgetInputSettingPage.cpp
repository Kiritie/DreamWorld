// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetInputSettingPage.h"

#include "InputMappingContext.h"
#include "PlayerMappableInputConfig.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Gameplay/DWGameMode.h"
#include "Input/DWInputModule.h"
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

	const TArray<FLoadedInputConfigMapping>& RegisteredConfigs = AInputModule::Get()->GetAllRegisteredInputConfigs();	
	const TMap<FName, FKey>& CustomKeyMap = AInputModule::Get()->GetCustomPlayerInputConfig();
	
	for (const FLoadedInputConfigMapping& InputConfigPair : RegisteredConfigs)
	{
		if (InputConfigPair.Type != ECommonInputType::MouseAndKeyboard)
		{
			continue;
		}
		
		TArray<FEnhancedActionKeyMapping> ConfigMappings = InputConfigPair.Config->GetPlayerMappableKeys();
		
		// Add each player mappable key to the settings screen!
		for (FEnhancedActionKeyMapping& Mapping : ConfigMappings)
		{
			FEnhancedActionKeyMapping MappingSynthesized(Mapping);

			// If the player has bound a custom key to this action, then set it to that
			if (const FKey* PlayerBoundKey = CustomKeyMap.Find(Mapping.PlayerMappableOptions.Name))
			{
				MappingSynthesized.Key = *PlayerBoundKey;
			}

			if (MappingSynthesized.PlayerMappableOptions.Name != NAME_None && !MappingSynthesized.PlayerMappableOptions.DisplayName.IsEmpty())
			{
				UWidgetSettingItemBase* SettingItem = nullptr;
				for(auto Iter : SettingItems)
				{
					if(Iter->GetLabel().EqualTo(MappingSynthesized.PlayerMappableOptions.DisplayName))
					{
						SettingItem = Iter;
						break;
					}
				}
				if(!SettingItem)
				{
					SettingItem = CreateSubWidget<UWidgetKeySettingItem>({ MappingSynthesized.PlayerMappableOptions.DisplayName }, UAssetModuleBPLibrary::GetStaticClass(FName("KeySettingItem")));
				}
				AddSettingItem(SettingItem, MappingSynthesized.PlayerMappableOptions.DisplayCategory, MappingSynthesized);
			}
		}
	}
}

void UWidgetInputSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	for(auto& Iter1 : ItemMappings)
	{
		ADWInputModule::Get()->GetAllMappingByDisplayName(Iter1.Key->GetLabel(), Iter1.Value);
		TArray<FParameter> Values;
		for(auto& Iter2 : Iter1.Value)
		{
			Values.Add(Iter2.Key.ToString());
		}
		Iter1.Key->SetValues(Values);
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

void UWidgetInputSettingPage::AddSettingItem(UWidgetSettingItemBase* InSettingItem, const FText& InCategory)
{
	Super::AddSettingItem(InSettingItem, InCategory);
}

void UWidgetInputSettingPage::AddSettingItem(UWidgetSettingItemBase* InSettingItem, const FText& InCategory, FEnhancedActionKeyMapping InActionMapping)
{
	AddSettingItem(InSettingItem, InCategory);
	ItemMappings.FindOrAdd(InSettingItem).Add(InActionMapping);
}

void UWidgetInputSettingPage::ClearSettingItems()
{
	Super::ClearSettingItems();
	ItemMappings.Empty();
}

FDWInputModuleSaveData& UWidgetInputSettingPage::GetDefaultInputData() const
{
	return USaveGameModuleBPLibrary::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().InputData;
}

bool UWidgetInputSettingPage::ChangeBinding(UWidgetSettingItemBase* InSettingItem, int32 InKeyBindSlot)
{
	TArray<FEnhancedActionKeyMapping>& Mappings = ItemMappings.FindOrAdd(InSettingItem);
	// Early out if they hit the same button that is already bound. This allows for them to exit binding if they made a mistake.
	if (Mappings[InKeyBindSlot].Key == NewKey)
	{
		return false;
	}
	
	if (!NewKey.IsGamepadKey())
	{
		ULocalPlayer* LocalPlayer = CastChecked<ULocalPlayer>(GetOwningLocalPlayer());
		AInputModule::Get()->AddOrUpdateCustomKeyboardBindings(Mappings[InKeyBindSlot].PlayerMappableOptions.Name, NewKey, LocalPlayer);
		Mappings[InKeyBindSlot].Key = NewKey;
		return true;
	}

	return false;
}

void UWidgetInputSettingPage::GetAllMappedActionsFromKey(FKey Key, TArray<FName>& OutActionNames) const
{
	AInputModule::Get()->GetAllMappingNamesFromKey(Key, OutActionNames);
}
