// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Generate/WidgetGeneratePanel.h"

#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Asset/AssetModuleStatics.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/ScrollBox.h"
#include "CommonButtonBase.h"
#include "Ability/Inventory/AbilityInventoryAgentInterface.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Achievement/AchievementModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Generate/WidgetGenerateItem.h"
#include "Widget/Item/WidgetAbilityItem.h"
#include "Widget/Item/Category/WidgetAbilityItemCategoryBar.h"

UWidgetGeneratePanel::UWidgetGeneratePanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GeneratePanel");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::UIOnly;

	WidgetType = EWidgetType::Temporary;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	bWidgetAutoFocus = true;
	
	SetIsFocusable(true);
}

void UWidgetGeneratePanel::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
	
	if(BtnGenerate)
	{
		BtnGenerate->OnClicked().AddUObject(this, &UWidgetGeneratePanel::OnGenerateButtonClicked);
	}
	if(CategoryBar)
	{
		CategoryBar->OnCategorySelected.AddDynamic(this, &UWidgetGeneratePanel::OnItemCategorySelected);
	}
}

void UWidgetGeneratePanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetGeneratePanel::OnReset(bool bForce)
{
	Super::OnReset(bForce);

	for(auto Iter : GenerateItems)
	{
		DestroySubWidget(Iter, true);
	}
	GenerateItems.Empty();

	for(auto Iter : PreviewItems)
	{
		DestroySubWidget(Iter, true);
	}
	PreviewItems.Empty();

	PreviewGenerateRawDataIndex = 0;
	SelectedGenerateItem = nullptr;
	SelectedGenerateRawData = FDWGenerateRawData();

	GetWorld()->GetTimerManager().ClearTimer(PreviewContentRefreshTH);
}

void UWidgetGeneratePanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	if(InParams.IsValidIndex(0))
	{
		GenerateToolID = InParams[0].GetObjectValue<IPrimaryEntityInterface>()->Execute_GetAssetID(InParams[0]);
	}
	
	Super::OnOpen(InParams, bInstant);

	if(GetOwnerObject<IAbilityInventoryAgentInterface>())
	{
		GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->OnRefresh.AddDynamic(this, &UWidgetGeneratePanel::Refresh);
	}

	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();

	if(CategoryBar)
	{
		CategoryBar->SetSelectedItemType(EAbilityItemType::None);
	}

	OnGenerateContentRefresh(true);
}

void UWidgetGeneratePanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	if(GetOwnerObject<IAbilityInventoryAgentInterface>())
	{
		GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->OnRefresh.RemoveDynamic(this, &UWidgetGeneratePanel::Refresh);
	}

	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();
}

void UWidgetGeneratePanel::OnRefresh()
{
	Super::OnRefresh();

	for(auto Iter : PreviewItems)
	{
		Iter->Refresh();
	}

	bool bCanGenerate = true;
	FDWGenerateItemData SelectedGenerateItemData;
	if(GetSelectedGenerateItemData(SelectedGenerateItemData))
	{
		UAbilityInventoryBase* Inventory = GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory();
		for(auto& Iter1 : SelectedGenerateItemData.RawDatas)
		{
			for(auto& Iter2 : Iter1.Raws)
			{
				const FItemQueryData ItemQueryData = Inventory->QueryItemByRange(EItemQueryType::Get, Iter2);
				if(ItemQueryData.Item.Count < Iter2.Count)
				{
					bCanGenerate = false;
					break;
				}
			}
			if(bCanGenerate)
			{
				SelectedGenerateRawData = Iter1;
				break;
			}
		}
	}
	else
	{
		bCanGenerate = false;
	}
	
	if(BtnGenerate)
	{
		BtnGenerate->SetIsEnabled(bCanGenerate);
	}
}

void UWidgetGeneratePanel::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);
}

void UWidgetGeneratePanel::OnItemCategorySelected_Implementation(EAbilityItemType InItemType)
{
	OnGenerateContentRefresh(true);
}

void UWidgetGeneratePanel::OnGenerateItemSelected_Implementation(UWidgetGenerateItem* InItem)
{
	if(InItem == SelectedGenerateItem) return;

	if(SelectedGenerateItem)
	{
		SelectedGenerateItem->SetIsSelected(false, false);
	}

	SelectedGenerateItem = InItem;

	Refresh();

	PreviewGenerateRawDataIndex = 0;
	GetWorld()->GetTimerManager().SetTimer(PreviewContentRefreshTH, FTimerDelegate::CreateUObject(this, &UWidgetGeneratePanel::OnPreviewContentRefresh), 1.5f, true, 0.f);
}

void UWidgetGeneratePanel::OnGenerateItemDeselected_Implementation(UWidgetGenerateItem* InItem)
{
	if(InItem != SelectedGenerateItem) return;

	SelectedGenerateItem = nullptr;

	Refresh();

	GetWorld()->GetTimerManager().ClearTimer(PreviewContentRefreshTH);
	for(auto Iter : PreviewItems)
	{
		DestroySubWidget(Iter, true);
	}
	PreviewItems.Empty();
}

void UWidgetGeneratePanel::OnGenerateContentRefresh(bool bScrollToStart)
{
	Reset();

	TArray<FDWGenerateItemData> GenerateItemDatas;
	if(GenerateContent && UAssetModuleStatics::ReadDataTable(GenerateItemDatas))
	{
		for(auto& Iter : GenerateItemDatas)
		{
			if(Iter.ToolID.IsValid() && Iter.ToolID != GenerateToolID || CategoryBar->GetSelectedItemType() != EAbilityItemType::None && Iter.Item.GetType() != CategoryBar->GetSelectedItemType()) continue;
			
			if(UWidgetGenerateItem* GenerateItem = CreateSubWidget<UWidgetGenerateItem>({ &Iter.Item, &Iter }, GenerateItemClass))
			{
				GenerateItems.Add(GenerateItem);
				if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(GenerateContent->AddChild(GenerateItem)))
				{
					ScrollBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				}
			}
		}
		
		if(bScrollToStart)
		{
			GenerateContent->ScrollToStart();
		}
	}
}

void UWidgetGeneratePanel::OnPreviewContentRefresh()
{
	if(PreviewContent)
	{
		FDWGenerateItemData _SelectedGenerateItemData;
		if(GetSelectedGenerateItemData(_SelectedGenerateItemData))
		{
			for(auto Iter : PreviewItems)
			{
				DestroySubWidget(Iter, true);
			}
			PreviewItems.Empty();
			for(auto& Iter : _SelectedGenerateItemData.RawDatas[PreviewGenerateRawDataIndex].Raws)
			{
				if(UWidgetAbilityItem* PreviewItem = CreateSubWidget<UWidgetAbilityItem>({ &Iter }, UAssetModuleStatics::GetStaticClass(FName("RequiredPreviewItem"))))
				{
					PreviewItems.Add(PreviewItem);
					if(UWrapBoxSlot* WrapBoxSlot = PreviewContent->AddChildToWrapBox(PreviewItem))
					{
						WrapBoxSlot->SetPadding(FMargin(5.f, 5.f, 5.f, 5.f));
					}
				}
			}
			if(_SelectedGenerateItemData.RawDatas.Num() == 1)
			{
				GetWorld()->GetTimerManager().ClearTimer(PreviewContentRefreshTH);
			}
			else if(++PreviewGenerateRawDataIndex >= _SelectedGenerateItemData.RawDatas.Num())
			{
				PreviewGenerateRawDataIndex = 0;
			}
		}
	}
}

void UWidgetGeneratePanel::OnGenerateButtonClicked()
{
	FDWGenerateItemData _SelectedGenerateItemData;
	if(GetSelectedGenerateItemData(_SelectedGenerateItemData))
	{
		UAbilityInventoryBase* Inventory = GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory();
		for(auto& Iter : SelectedGenerateRawData.Raws)
		{
			Inventory->RemoveItemByRange(Iter);
		}
		Inventory->AddItemByRange(_SelectedGenerateItemData.Item, -1);
		if(_SelectedGenerateItemData.Item.Count > 0)
		{
			GetOwnerObject<IAbilityInventoryAgentInterface>()->OnDiscardItem(_SelectedGenerateItemData.Item, false);
		}
		UAchievementModuleStatics::UnlockAchievement(FName("FirstGenerateItem"));
	}
}

bool UWidgetGeneratePanel::GetSelectedGenerateItemData(FDWGenerateItemData& OutItemData) const
{
	if(SelectedGenerateItem)
	{
		OutItemData = SelectedGenerateItem->GetGenerateItemData();
		return true;
	}
	return false;
}
