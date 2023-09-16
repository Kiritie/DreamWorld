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
#include "Widget/Inventory/Item/WidgetInventoryGenerateItem.h"
#include "Widget/Inventory/Item/WidgetInventoryPreviewItem.h"
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
	WidgetInputMode = EInputMode::UIOnly;

	WidgetType = EWidgetType::Temporary;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	SetIsFocusable(true);

	GenerateSlotClass = LoadClass<UWidgetInventoryGenerateItem>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Item/WB_InventoryGenerateItem.WB_InventoryGenerateItem_C'"));
	PreviewSlotClass = LoadClass<UWidgetInventoryPreviewItem>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Inventory/Item/WB_InventoryPreviewItem.WB_InventoryPreviewItem_C'"));
}

void UWidgetGeneratePanel::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);

	if(BtnGenerate)
	{
		BtnGenerate->OnClicked.AddDynamic(this, &UWidgetGeneratePanel::OnGenerateButtonClicked);
	}
}

void UWidgetGeneratePanel::OnInitialize_Implementation(UObject* InOwner)
{
	if(OwnerObject == InOwner) return;
	
	if(OwnerObject)
	{
		UInventory* Inventory = Cast<ADWCharacter>(OwnerObject)->GetInventory();
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
		for(auto Iter : GenerateItems)
		{
			DestroySubWidget(Iter, true);
		}
		GenerateItems.Empty();
		for(int32 i = 0; i < GenerateItemDatas.Num(); i++)
		{
			if(UWidgetInventoryGenerateItem* GenerateItem = CreateSubWidget<UWidgetInventoryGenerateItem>({ &GenerateItemDatas[i].Item }, GenerateSlotClass))
			{
				if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(GenerateContent->AddChild(GenerateItem)))
				{
					ScrollBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				}
				GenerateItems.Add(GenerateItem);
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

	for(auto Iter : PreviewItems)
	{
		Iter->OnRefresh();
	}

	if(BtnGenerate)
	{
		bool bCanGenerate = true;
		FDWGenerateItemData GenerateItemData;
		if(GetSelectedGenerateItemData(GenerateItemData))
		{
			UInventory* Inventory = Cast<ADWCharacter>(OwnerObject)->GetInventory();
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
	
	for(auto Iter : GenerateItems)
	{
		DestroySubWidget(Iter, true);
	}

	for(auto Iter : PreviewItems)
	{
		DestroySubWidget(Iter, true);
	}
}

void UWidgetGeneratePanel::OnGenerateSlotSelected(UWidgetInventoryGenerateItem* InSlot)
{
	if(InSlot == SelectedGenerateSlot) return;

	if(SelectedGenerateSlot)
	{
		SelectedGenerateSlot->OnUnSelected();
	}

	SelectedGenerateSlot = InSlot;

	if(SelectedGenerateSlot)
	{
		if(PreviewContent)
		{
			FDWGenerateItemData GenerateItemData;
			if(GetSelectedGenerateItemData(GenerateItemData))
			{
				for(auto Iter : PreviewItems)
				{
					DestroySubWidget(Iter, true);
				}
				PreviewItems.Empty();
				for(int32 i = 0; i < GenerateItemData.Raws.Num(); i++)
				{
					if(UWidgetInventoryPreviewItem* PreviewItem = CreateSubWidget<UWidgetInventoryPreviewItem>({ &GenerateItemData.Raws[i] }, PreviewSlotClass))
					{
						if(UWrapBoxSlot* WrapBoxSlot = PreviewContent->AddChildToWrapBox(PreviewItem))
						{
							WrapBoxSlot->SetPadding(FMargin(5.f, 5.f, 5.f, 5.f));
						}
						PreviewItems.Add(PreviewItem);
					}
				}
			}
		}
	}
	else
	{
		for(auto Iter : PreviewItems)
		{
			DestroySubWidget(Iter, true);
		}
		PreviewItems.Empty();
	}

	Refresh();
}

void UWidgetGeneratePanel::OnGenerateButtonClicked()
{
	FDWGenerateItemData GenerateItemData;
	if(GetSelectedGenerateItemData(GenerateItemData))
	{
		UInventory* Inventory = Cast<ADWCharacter>(OwnerObject)->GetInventory();
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
		const int32 tmpIndex = GenerateItems.Find(SelectedGenerateSlot);
		if(GenerateItemDatas.IsValidIndex(tmpIndex))
		{
			OutItemData = GenerateItemDatas[tmpIndex];
			return true;
		}
	}
	return false;
}
