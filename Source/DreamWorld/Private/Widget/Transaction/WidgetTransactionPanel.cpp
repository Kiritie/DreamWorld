// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Transaction/WidgetTransactionPanel.h"

#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Asset/AssetModuleStatics.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/ScrollBox.h"
#include "CommonButtonBase.h"
#include "Ability/Inventory/AbilityInventoryAgentInterface.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Item/AbilityTradableItemDataBase.h"
#include "Achievement/AchievementModuleStatics.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Common/CommonButtonGroup.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Item/WidgetAbilityItem.h"
#include "Widget/Transaction/WidgetTransactionItem.h"

UWidgetTransactionPanel::UWidgetTransactionPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("TransactionPanel");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::UIOnly;

	WidgetType = EWidgetType::Temporary;
	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	SetIsFocusable(true);
}

void UWidgetTransactionPanel::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
	
	if(BtnTransaction)
	{
		BtnTransaction->OnClicked().AddUObject(this, &UWidgetTransactionPanel::OnTransactionButtonClicked);
	}

	TabGroup = UObjectPoolModuleStatics::SpawnObject<UCommonButtonGroup>();
	TabGroup->SetSelectionRequiredN(true);
	TabGroup->OnSelectedButtonBaseChanged.AddDynamic(this, &UWidgetTransactionPanel::OnTabButtonSelected);

	if(TabContent)
	{
		for(auto Iter : TabContent->GetAllChildren())
		{
			TabGroup->AddWidget(Iter);
		}
	}
}

void UWidgetTransactionPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	if(GetOwnerObject<IAbilityInventoryAgentInterface>())
	{
		GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->OnRefresh.RemoveDynamic(this, &UWidgetTransactionPanel::Refresh);
	}

	Super::OnInitialize(InOwner, InParams);

	if(GetOwnerObject<IAbilityInventoryAgentInterface>())
	{
		GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->OnRefresh.AddDynamic(this, &UWidgetTransactionPanel::Refresh);
	}
}

void UWidgetTransactionPanel::OnReset(bool bForce)
{
	Super::OnReset(bForce);

	for(auto Iter : TransactionItems)
	{
		DestroySubWidget(Iter, true);
	}
	TransactionItems.Empty();

	for(auto Iter : PreviewItems)
	{
		DestroySubWidget(Iter, true);
	}
	PreviewItems.Empty();

	SelectedTransactionItem = nullptr;
	SelectedPreviewItems = TArray<FAbilityItem>();
}

void UWidgetTransactionPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();

	TransactionTarget = InParams[0].GetObjectValue<IAbilityInventoryAgentInterface>();
	TransactionTarget->GetInventory()->OnRefresh.AddDynamic(this, &UWidgetTransactionPanel::Refresh);

	TabGroup->SelectButtonAtIndex(0);
	
	OnTransactionContentRefresh();
}

void UWidgetTransactionPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();

	TransactionTarget->GetInventory()->OnRefresh.RemoveDynamic(this, &UWidgetTransactionPanel::Refresh);
	TransactionTarget = nullptr;
}

void UWidgetTransactionPanel::OnRefresh()
{
	Super::OnRefresh();

	for(auto Iter : PreviewItems)
	{
		Iter->OnRefresh();
	}

	bool bCanTransaction = true;
	FAbilityItem _SelectedTransactionItem;
	if(GetSelectedTransactionItem(_SelectedTransactionItem))
	{
		UAbilityInventoryBase* Inventory = GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory();
		SelectedPreviewItems = _SelectedTransactionItem.GetData<UAbilityTradableItemDataBase>().Prices;
		if(GetTabIndex() == 0)
		{
			for(auto& Iter1 : SelectedPreviewItems)
			{
				const FItemQueryInfo ItemQueryInfo = Inventory->QueryItemByRange(EItemQueryType::Get, Iter1);
				if(ItemQueryInfo.Item.Count < Iter1.Count)
				{
					bCanTransaction = false;
					break;
				}
			}
		}
	}
	else
	{
		bCanTransaction = false;
	}
	
	if(BtnTransaction)
	{
		BtnTransaction->SetIsEnabled(bCanTransaction);
	}
}

void UWidgetTransactionPanel::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);
}

void UWidgetTransactionPanel::OnTabButtonSelected_Implementation(UCommonButtonBase* SelectedTabButton, int32 ButtonIndex)
{
	if(TransactionTarget)
	{
		OnTransactionContentRefresh();
	}
}

void UWidgetTransactionPanel::OnTransactionContentRefresh()
{
	Reset();
	
	if(TransactionContent)
	{
		IAbilityInventoryAgentInterface* Seller = GetTabIndex() == 0 ? TransactionTarget : GetOwnerObject<IAbilityInventoryAgentInterface>();

		for(auto Iter : Seller->GetInventory()->GetSlots())
		{
			if(Iter->IsEmpty() || !Iter->GetItem().IsDataType<UAbilityTradableItemDataBase>() || Iter->GetItem().GetData<UAbilityTradableItemDataBase>().Prices.IsEmpty()) continue;
			
			if(UWidgetTransactionItem* TransactionItem = CreateSubWidget<UWidgetTransactionItem>({ &Iter->GetItem() }, UAssetModuleStatics::GetStaticClass(FName("TransactionItem"))))
			{
				if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(TransactionContent->AddChild(TransactionItem)))
				{
					ScrollBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				}
				TransactionItems.Add(TransactionItem);
			}
		}
	}

	Refresh();
}

void UWidgetTransactionPanel::OnTransactionItemSelected_Implementation(UWidgetTransactionItem* InItem)
{
	if(InItem == SelectedTransactionItem) return;

	if(SelectedTransactionItem)
	{
		SelectedTransactionItem->SetIsSelected(false, false);
	}

	SelectedTransactionItem = InItem;

	Refresh();

	OnPreviewContentRefresh();
}

void UWidgetTransactionPanel::OnTransactionItemDeselected_Implementation(UWidgetTransactionItem* InItem)
{
	if(InItem != SelectedTransactionItem) return;

	SelectedTransactionItem = nullptr;

	Refresh();

	for(auto Iter : PreviewItems)
	{
		DestroySubWidget(Iter, true);
	}
	PreviewItems.Empty();
}

void UWidgetTransactionPanel::OnPreviewContentRefresh()
{
	if(PreviewContent)
	{
		FAbilityItem _SelectedTransactionItem;
		if(GetSelectedTransactionItem(_SelectedTransactionItem))
		{
			for(auto Iter : PreviewItems)
			{
				DestroySubWidget(Iter, true);
			}
			PreviewItems.Empty();
			for(auto& Iter : SelectedPreviewItems)
			{
				if(UWidgetAbilityItem* PreviewItem = CreateSubWidget<UWidgetAbilityItem>({ &Iter }, UAssetModuleStatics::GetStaticClass(GetTabIndex() == 0 ? FName("RequiredPreviewItem") : FName("SimplePreviewItem"))))
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

void UWidgetTransactionPanel::OnTransactionButtonClicked()
{
	FAbilityItem _SelectedTransactionItem;
	if(GetSelectedTransactionItem(_SelectedTransactionItem))
	{
		IAbilityInventoryAgentInterface* Buyer = GetTabIndex() == 0 ? GetOwnerObject<IAbilityInventoryAgentInterface>() : TransactionTarget;
		IAbilityInventoryAgentInterface* Seller = GetTabIndex() == 0 ? TransactionTarget : GetOwnerObject<IAbilityInventoryAgentInterface>();
		
		FAbilityItem _TransactionItem = FAbilityItem(_SelectedTransactionItem, 1);
		TArray<FAbilityItem> _PreviewItems = SelectedPreviewItems;

		for(auto& Iter : _PreviewItems)
		{
			Buyer->GetInventory()->RemoveItemByRange(Iter);
		}
		Buyer->GetInventory()->AddItemByRange(_TransactionItem, -1);
		if(_TransactionItem.Count > 0)
		{
			Buyer->OnDiscardItem(_TransactionItem, false);
		}
		
		_TransactionItem = FAbilityItem(_SelectedTransactionItem, 1);
		_PreviewItems = SelectedPreviewItems;
		Seller->GetInventory()->RemoveItemByRange(_TransactionItem);
		for(auto& Iter : _PreviewItems)
		{
			Seller->GetInventory()->AddItemByRange(Iter);
		}
		if(Seller->GetInventory()->QueryItemByRange(EItemQueryType::Get, _TransactionItem).Item.Count <= 0)
		{
			OnTransactionContentRefresh();
		}
		else
		{
			_SelectedTransactionItem.Count--;
			SelectedTransactionItem->Init({ &_SelectedTransactionItem });
		}

		UAchievementModuleStatics::UnlockAchievement(FName("FirstTransactionItem"));
	}
}

int32 UWidgetTransactionPanel::GetTabIndex() const
{
	return TabGroup->GetSelectedButtonIndex();
}

bool UWidgetTransactionPanel::GetSelectedTransactionItem(FAbilityItem& OutItemData) const
{
	if(SelectedTransactionItem)
	{
		OutItemData = SelectedTransactionItem->GetItem();
		return true;
	}
	return false;
}
