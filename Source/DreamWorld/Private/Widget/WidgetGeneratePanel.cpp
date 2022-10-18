// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetGeneratePanel.h"

#include "AchievementSubSystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Global/GlobalBPLibrary.h"
#include "Ability/Inventory/CharacterInventory.h"
#include "Ability/Inventory/Slot/InventorySlot.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Widget/Inventory/Slot/Preview/WidgetInventoryGenerateSlot.h"
#include "Widget/Inventory/Slot/Preview/WidgetInventoryPreviewSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/ScrollBox.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"
#include "Components/Button.h"
#include "Ability/Inventory/Inventory.h"
#include "Character/DWCharacter.h"
#include "Gameplay/WHGameInstance.h"

UWidgetGeneratePanel::UWidgetGeneratePanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GeneratePanel");
	ParentName = FName("GameHUD");
	InputMode = EInputMode::UIOnly;

	WidgetType = EWidgetType::Temporary;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	bIsFocusable = true;

	GenerateSlotClass = LoadClass<UWidgetInventoryGenerateSlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/Preview/WB_InventoryGenerateSlot.WB_InventoryGenerateSlot_C'"));
	PreviewSlotClass = LoadClass<UWidgetInventoryPreviewSlot>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Slot/Preview/WB_InventoryPreviewSlot.WB_InventoryPreviewSlot_C'"));
}

void UWidgetGeneratePanel::OnCreate_Implementation(AActor* InOwner)
{
	Super::OnCreate_Implementation(InOwner);

	if(BtnGenerate)
	{
		BtnGenerate->OnClicked.AddDynamic(this, &UWidgetGeneratePanel::OnGenerateButtonClicked);
	}
}

void UWidgetGeneratePanel::OnInitialize_Implementation(AActor* InOwner)
{
	if(OwnerActor)
	{
		UInventory* Inventory = Cast<ADWCharacter>(OwnerActor)->GetInventory();
		Inventory->OnRefresh.RemoveDynamic(this, &UWidgetGeneratePanel::Refresh);
	}

	Super::OnInitialize_Implementation(InOwner);

	if(InOwner)
	{
		UInventory* Inventory = Cast<ADWCharacter>(InOwner)->GetInventory();
		Inventory->OnRefresh.AddDynamic(this, &UWidgetGeneratePanel::Refresh);
	}
}

void UWidgetGeneratePanel::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	TArray<FDWGenerateItemData> GenerateItemDatas;
	if(GenerateContent && UAssetModuleBPLibrary::ReadDataTable(GenerateItemDatas))
	{
		for(auto Iter : GenerateSlots)
		{
			UObjectPoolModuleBPLibrary::DespawnObject(Iter);
		}
		GenerateSlots.Empty();
		for(int32 i = 0; i < GenerateItemDatas.Num(); i++)
		{
			if(UWidgetInventoryGenerateSlot* GenerateSlot = UObjectPoolModuleBPLibrary::SpawnObject<UWidgetInventoryGenerateSlot>(nullptr, GenerateSlotClass))
			{
				GenerateSlot->OnInitialize(this, GenerateItemDatas[i].Item);
				GenerateSlot->OnSlotSelected.AddDynamic(this, &UWidgetGeneratePanel::OnGenerateSlotSelected);
				if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(GenerateContent->AddChild(GenerateSlot)))
				{
					ScrollBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				}
				GenerateSlots.Add(GenerateSlot);
			}
		}
		OnGenerateSlotSelected(nullptr);
	}

	Refresh();
}

void UWidgetGeneratePanel::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

void UWidgetGeneratePanel::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();

	for(auto Iter : PreviewSlots)
	{
		Iter->OnRefresh();
	}

	if(BtnGenerate)
	{
		bool bCanGenerate = true;
		FDWGenerateItemData GenerateItemData;
		if(GetSelectedGenerateItemData(GenerateItemData))
		{
			UInventory* Inventory = Cast<ADWCharacter>(OwnerActor)->GetInventory();
			for(auto Iter : GenerateItemData.Raws)
			{
				const FQueryItemInfo QueryItemInfo = Inventory->QueryItemByRange(EQueryItemType::Get, Iter);
				if(QueryItemInfo.Item.Count < Iter.Count)
				{
					bCanGenerate = false;
					break;
				}
			}
		}
		else
		{
			bCanGenerate = false;
		}
		BtnGenerate->SetIsEnabled(bCanGenerate);
	}
}

void UWidgetGeneratePanel::OnDestroy_Implementation(bool bRecovery)
{
	Super::OnDestroy_Implementation(bRecovery);
	
	for(auto Iter : GenerateSlots)
	{
		if(Iter->IsRooted())
		{
			Iter->RemoveFromRoot();
		}
		Iter->ConditionalBeginDestroy();
	}

	for(auto Iter : PreviewSlots)
	{
		if(Iter->IsRooted())
		{
			Iter->RemoveFromRoot();
		}
		Iter->ConditionalBeginDestroy();
	}
}

void UWidgetGeneratePanel::OnGenerateSlotSelected(UWidgetInventoryGenerateSlot* InSlot)
{
	if(InSlot == SelectedGenerateSlot) return;

	if(SelectedGenerateSlot)
	{
		SelectedGenerateSlot->SetSelectState(false);
	}

	SelectedGenerateSlot = InSlot;

	if(SelectedGenerateSlot)
	{
		SelectedGenerateSlot->SetSelectState(true);

		if(PreviewContent)
		{
			FDWGenerateItemData GenerateItemData;
			if(GetSelectedGenerateItemData(GenerateItemData))
			{
				for(auto Iter : PreviewSlots)
				{
					UObjectPoolModuleBPLibrary::DespawnObject(Iter);
				}
				PreviewSlots.Empty();
				for(int32 i = 0; i < GenerateItemData.Raws.Num(); i++)
				{
					if(UWidgetInventoryPreviewSlot* PreviewSlot = UObjectPoolModuleBPLibrary::SpawnObject<UWidgetInventoryPreviewSlot>(nullptr, PreviewSlotClass))
					{
						PreviewSlot->OnInitialize(this, GenerateItemData.Raws[i]);
						if(UWrapBoxSlot* WrapBoxSlot = PreviewContent->AddChildToWrapBox(PreviewSlot))
						{
							WrapBoxSlot->SetPadding(FMargin(5.f, 5.f, 5.f, 5.f));
						}
						PreviewSlots.Add(PreviewSlot);
					}
				}
			}
		}
	}
	else
	{
		for(auto Iter : PreviewSlots)
		{
			UObjectPoolModuleBPLibrary::DespawnObject(Iter);
		}
		PreviewSlots.Empty();
	}

	Refresh();
}

void UWidgetGeneratePanel::OnGenerateButtonClicked()
{
	FDWGenerateItemData GenerateItemData;
	if(GetSelectedGenerateItemData(GenerateItemData))
	{
		UInventory* Inventory = Cast<ADWCharacter>(OwnerActor)->GetInventory();
		for(auto& Iter : GenerateItemData.Raws)
		{
			Inventory->RemoveItemByRange(Iter);
		}
		Inventory->AddItemByRange(GenerateItemData.Item, -1);

		UGlobalBPLibrary::GetGameInstance()->GetSubsystem<UAchievementSubSystem>()->Unlock(FName("FirstGenerateItem"));
	}
}

bool UWidgetGeneratePanel::GetSelectedGenerateItemData(FDWGenerateItemData& OutItemData)
{
	TArray<FDWGenerateItemData> GenerateItemDatas;
	if(SelectedGenerateSlot && UAssetModuleBPLibrary::ReadDataTable(GenerateItemDatas))
	{
		const int32 tmpIndex = GenerateSlots.Find(SelectedGenerateSlot);
		if(GenerateItemDatas.IsValidIndex(tmpIndex))
		{
			OutItemData = GenerateItemDatas[tmpIndex];
			return true;
		}
	}
	return false;
}
