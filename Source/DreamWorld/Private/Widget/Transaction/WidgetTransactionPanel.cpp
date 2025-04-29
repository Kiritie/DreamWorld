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
#include "Ability/Item/AbilityTransItemDataBase.h"
#include "Achievement/AchievementModuleStatics.h"
#include "Item/Prop/Blueprint/DWPropBlueprintData.h"
#include "Setting/Widget/Item/WidgetOptionSettingItemBase.h"
#include "Voxel/VoxelModule.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Common/CommonButtonGroup.h"
#include "Widget/Common/WidgetProgressBox.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Item/WidgetAbilityItem.h"
#include "Widget/Item/Category/WidgetAbilityItemCategoryBar.h"
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

	bWidgetAutoFocus = true;

	SetIsFocusable(true);

	TransactionCraftMap = {
		{
			EDWTransactionType::Buy, EDWCharacterCraft::Merchant
		},
		{
			EDWTransactionType::Sell, EDWCharacterCraft::Merchant
		},
		{
			EDWTransactionType::Upgrade, EDWCharacterCraft::Blacksmith
		},
		{
			EDWTransactionType::Generate, EDWCharacterCraft::Blacksmith
		},
		{
			EDWTransactionType::Split, EDWCharacterCraft::Blacksmith
		},
	};
}

void UWidgetTransactionPanel::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
	
	if(BtnTransaction)
	{
		BtnTransaction->OnClicked().AddUObject(this, &UWidgetTransactionPanel::OnTransactionButtonClicked);
	}
	if(CategoryBar)
	{
		CategoryBar->OnCategorySelected.AddDynamic(this, &UWidgetTransactionPanel::OnItemCategorySelected);
	}
	if(TransactionNumOption)
	{
		TransactionNumOption->OnValueChanged.AddDynamic(this, &UWidgetTransactionPanel::OnTransactionNumOptionValueChange);
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
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetTransactionPanel::OnReset(bool bForce)
{
	Super::OnReset(bForce);

	OnTransactionItemDeselected(SelectedTransactionItem);

	for(auto Iter : TransactionItems)
	{
		DestroySubWidget(Iter, true);
	}
	TransactionItems.Empty();
}

void UWidgetTransactionPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	TransactionTarget = InParams[0];
	TransactionTarget->GetInventory()->OnRefresh.AddDynamic(this, &UWidgetTransactionPanel::Refresh);
	
	if(CategoryBar)
	{
		CategoryBar->SetSelectedItemType(EAbilityItemType::None);
	}

	OnTransactionContentRefresh(true);

	Super::OnOpen(InParams, bInstant);

	if(GetOwnerObject<IAbilityInventoryAgentInterface>())
	{
		GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->OnRefresh.AddDynamic(this, &UWidgetTransactionPanel::Refresh);
	}

	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();
}

void UWidgetTransactionPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	if(GetOwnerObject<IAbilityInventoryAgentInterface>())
	{
		GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->OnRefresh.RemoveDynamic(this, &UWidgetTransactionPanel::Refresh);
	}

	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();

	TransactionTarget->GetInventory()->OnRefresh.RemoveDynamic(this, &UWidgetTransactionPanel::Refresh);
	TransactionTarget = nullptr;
}

void UWidgetTransactionPanel::OnRefresh()
{
	Super::OnRefresh();

	for(auto Iter : PreviewItems)
	{
		Iter->Refresh();
	}

	bool bCanTransaction;
	FAbilityItem _SelectedTransactionItem;
	if(GetSelectedTransactionItem(_SelectedTransactionItem, bCanTransaction))
	{
		switch(GetSelectedTabType())
		{
			case EDWTransactionType::Buy:
			case EDWTransactionType::Sell:
			{
				SelectedPreviewItems = _SelectedTransactionItem.GetData<UAbilityTransItemDataBase>().Prices;
				break;
			}
			case EDWTransactionType::Upgrade:
			{
				SelectedPreviewItems = _SelectedTransactionItem.GetData<UAbilityTransItemDataBase>().Upgrades;
				break;
			}
			case EDWTransactionType::Generate:
			{
				SelectedPreviewItems = _SelectedTransactionItem.GetData<UDWPropBlueprintData>().GenerateRawDatas[GenerateRawDataIndex].Raws;
				break;
			}
			case EDWTransactionType::Split:
			{
				SelectedPreviewItems = _SelectedTransactionItem.GetData<UAbilityTransItemDataBase>().Splits;
				break;
			}
			default: break;
		}
		for(auto& Iter : SelectedPreviewItems)
		{
			Iter.Count *= GetSelectedTransactionNum();
		}
		switch(GetSelectedTabType())
		{
			case EDWTransactionType::Buy:
			case EDWTransactionType::Upgrade:
			case EDWTransactionType::Generate:
			{
				UAbilityInventoryBase* Inventory = GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory();
				for(auto& Iter : SelectedPreviewItems)
				{
					const FItemQueryData ItemQueryData = Inventory->QueryItemByRange(EItemQueryType::Get, Iter);
					if(ItemQueryData.Item.Count < Iter.Count)
					{
						bCanTransaction = false;
						break;
					}
				}
				break;
			}
			default: break;
		}
	}
	
	if(BtnTransaction)
	{
		// BtnTransaction->SetIsEnabled(bCanTransaction);
		BtnTransaction->SetRenderOpacity(bCanTransaction ? 1.f : 0.5f);
		BtnTransaction->SetVisibility(bCanTransaction ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::HitTestInvisible);
	}

	OnPreviewContentRefresh();
}

void UWidgetTransactionPanel::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);
}

void UWidgetTransactionPanel::OnItemCategorySelected_Implementation(EAbilityItemType InItemType)
{
	OnTransactionContentRefresh(true);
}

void UWidgetTransactionPanel::OnTabButtonSelected_Implementation(UCommonButtonBase* AssociatedButton, int32 ButtonIndex)
{
	if(CategoryBar)
	{
		CategoryBar->SetSelectedItemType(EAbilityItemType::None);
	}
	if(TransactionTarget)
	{
		OnTransactionContentRefresh(true);
	}
}

void UWidgetTransactionPanel::OnTransactionItemSelected_Implementation(UWidgetTransactionItem* InItem)
{
	if(InItem == SelectedTransactionItem) return;

	if(SelectedTransactionItem)
	{
		SelectedTransactionItem->SetIsSelected(false, false);
	}

	SelectedTransactionItem = InItem;

	GenerateRawDataIndex = 0;
	
	if(TransactionNumOption)
	{
		TArray<FString> Options;
		DON_WITHINDEX(30, i, Options.Add(FString::FromInt(i + 1)); )
		TransactionNumOption->SetOptionNames(Options);
	}

	Refresh();

	GetWorld()->GetTimerManager().SetTimer(GenerateRawDataRefreshTH, FTimerDelegate::CreateUObject(this, &UWidgetTransactionPanel::OnGenerateRawDataRefresh), 1.5f, true);
}

void UWidgetTransactionPanel::OnTransactionItemDeselected_Implementation(UWidgetTransactionItem* InItem)
{
	if(InItem != SelectedTransactionItem) return;

	SelectedTransactionItem = nullptr;

	GenerateRawDataIndex = 0;

	SelectedPreviewItems = TArray<FAbilityItem>();

	Refresh();

	for(auto Iter : PreviewItems)
	{
		DestroySubWidget(Iter, true);
	}
	PreviewItems.Empty();

	GetWorld()->GetTimerManager().ClearTimer(GenerateRawDataRefreshTH);
}

void UWidgetTransactionPanel::OnTransactionContentRefresh(bool bScrollToStart)
{
	Reset();
	
	if(TransactionContent)
	{
		TArray<FAbilityItem> _TransactionItems;
		switch(GetSelectedTabType())
		{
			case EDWTransactionType::Buy:
			{
				for(auto& Iter : TransactionTarget->GetInventory()->GetAllItems(true))
				{
					if(!Iter.IsDataType<UAbilityTransItemDataBase>() || Iter.GetData<UAbilityTransItemDataBase>().Prices.IsEmpty()) continue;

					_TransactionItems.Add(Iter);
				}
				break;
			}
			case EDWTransactionType::Sell:
			{
				for(auto& Iter : GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->GetAllItems(true))
				{
					if(!Iter.IsDataType<UAbilityTransItemDataBase>() || Iter.GetData<UAbilityTransItemDataBase>().Prices.IsEmpty()) continue;

					_TransactionItems.Add(Iter);
				}
				break;
			}
			case EDWTransactionType::Upgrade:
			{
				for(auto& Iter : GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->GetAllItems())
				{
					if(!Iter.IsDataType<UAbilityTransItemDataBase>() || Iter.GetData<UAbilityTransItemDataBase>().Upgrades.IsEmpty() || Iter.Level == Iter.GetData<UAbilityTransItemDataBase>().MaxLevel) continue;

					_TransactionItems.Add(Iter);
				}
				break;
			}
			case EDWTransactionType::Generate:
			{
				for(auto& Iter : GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->GetAllItems())
				{
					if(!Iter.IsDataType<UDWPropBlueprintData>() || Iter.GetData<UDWPropBlueprintData>().GenerateItemType == EAbilityItemType::None) continue;

					_TransactionItems.Add(Iter);
				}
				break;
			}
			case EDWTransactionType::Split:
			{
				for(auto& Iter : GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->GetAllItems())
				{
					if(!Iter.IsDataType<UAbilityTransItemDataBase>() || Iter.GetData<UAbilityTransItemDataBase>().Splits.IsEmpty()) continue;

					_TransactionItems.Add(Iter);
				}
				break;
			}
			default: break;
		}
		
		for(auto& Iter : _TransactionItems)
		{
			if(CategoryBar->GetSelectedItemType() != EAbilityItemType::None && Iter.GetType() != CategoryBar->GetSelectedItemType()) continue;
			
			if(UWidgetTransactionItem* TransactionItem = CreateSubWidget<UWidgetTransactionItem>({ &Iter }, TransactionItemClass))
			{
				TransactionItems.Add(TransactionItem);
				if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(TransactionContent->AddChild(TransactionItem)))
				{
					ScrollBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f));
				}
			}
		}
		
		if(bScrollToStart)
		{
			TransactionContent->ScrollToStart();
		}
	}

	Refresh();
}

void UWidgetTransactionPanel::OnGenerateRawDataRefresh()
{
	bool bCanTransaction;
	FAbilityItem _SelectedTransactionItem;
	if(GetSelectedTransactionItem(_SelectedTransactionItem, bCanTransaction))
	{
		switch(GetSelectedTabType())
		{
			case EDWTransactionType::Generate:
			{
				if(++GenerateRawDataIndex >= _SelectedTransactionItem.GetData<UDWPropBlueprintData>().GenerateRawDatas.Num())
				{
					GenerateRawDataIndex = 0;
				}
				Refresh();
				break;
			}
			default: break;
		}
	}
}

void UWidgetTransactionPanel::OnPreviewContentRefresh()
{
	if(PreviewContent)
	{
		bool bCanTransaction;
		FAbilityItem _SelectedTransactionItem;
		if(GetSelectedTransactionItem(_SelectedTransactionItem, bCanTransaction))
		{
			for(auto Iter : PreviewItems)
			{
				DestroySubWidget(Iter, true);
			}
			PreviewItems.Empty();
			for(auto& Iter : SelectedPreviewItems)
			{
				if(UWidgetAbilityItem* PreviewItem = CreateSubWidget<UWidgetAbilityItem>({ &Iter }, UAssetModuleStatics::GetStaticClass((GetSelectedTabType() == EDWTransactionType::Buy ||
					GetSelectedTabType() == EDWTransactionType::Upgrade || GetSelectedTabType() == EDWTransactionType::Generate) ? FName("RequiredPreviewItem") : FName("SimplePreviewItem"))))
				{
					PreviewItems.Add(PreviewItem);
					if(UWrapBoxSlot* WrapBoxSlot = PreviewContent->AddChildToWrapBox(PreviewItem))
					{
						WrapBoxSlot->SetPadding(FMargin(5.f, 5.f, 5.f, 5.f));
					}
				}
			}
		}
	}
}

void UWidgetTransactionPanel::OnTransactionButtonClicked()
{
	bool bCanTransaction;
	FAbilityItem _SelectedTransactionItem;
	if(GetSelectedTransactionItem(_SelectedTransactionItem, bCanTransaction))
	{
		switch(GetSelectedTabType())
		{
			case EDWTransactionType::Buy:
			case EDWTransactionType::Sell:
			{
				IAbilityInventoryAgentInterface* Buyer = GetSelectedTabType() == EDWTransactionType::Buy ? GetOwnerObject<IAbilityInventoryAgentInterface>() : TransactionTarget;
				IAbilityInventoryAgentInterface* Seller = GetSelectedTabType() == EDWTransactionType::Buy ? TransactionTarget : GetOwnerObject<IAbilityInventoryAgentInterface>();
		
				FAbilityItem _TransactionItem = _SelectedTransactionItem;
				TArray<FAbilityItem> _PreviewItems = SelectedPreviewItems;

				for(auto Iter : _PreviewItems)
				{
					Buyer->GetInventory()->RemoveItemByRange(Iter);
				}
				Buyer->GetInventory()->AddItemByRange(_TransactionItem, -1);
		
				_TransactionItem = _SelectedTransactionItem;
				Seller->GetInventory()->RemoveItemByRange(_TransactionItem);
				for(auto Iter : _PreviewItems)
				{
					Seller->GetInventory()->AddItemByRange(Iter);
				}
		
				OnTransactionContentRefresh();
				break;
			}
			case EDWTransactionType::Upgrade:
			{
				IAbilityInventoryAgentInterface* Buyer = GetOwnerObject<IAbilityInventoryAgentInterface>();
				IAbilityInventoryAgentInterface* Seller = TransactionTarget;

				TArray<FAbilityItem> _PreviewItems = SelectedPreviewItems;

				for(auto Iter : _PreviewItems)
				{
					Buyer->GetInventory()->RemoveItemByRange(Iter);
				}
			
				for(auto Iter : _PreviewItems)
				{
					Seller->GetInventory()->AddItemByRange(Iter);
				}

				FSimpleDelegate OnCompleted = FSimpleDelegate::CreateLambda([this](){
					bool bCanTransaction;
					FAbilityItem _SelectedTransactionItem;
					if(GetSelectedTransactionItem(_SelectedTransactionItem, bCanTransaction))
					{
						if(_SelectedTransactionItem.GetPayload<UAbilityInventorySlotBase>())
						{
							_SelectedTransactionItem.GetPayload<UAbilityInventorySlotBase>()->SetItem(_SelectedTransactionItem);
						}
						OnTransactionContentRefresh();
					}
				});
				UWidgetModuleStatics::OpenUserWidget<UWidgetProgressBox>({ FText::FromString(FString::Printf(TEXT("%s×%d"), *_SelectedTransactionItem.GetData().Name.ToString(), GetSelectedTransactionNum())), FText::FromString(TEXT("升级物品")), GetSelectedTransactionNum() * 1.f, true, &OnCompleted });
				break;
			}
			case EDWTransactionType::Generate:
			{
				IAbilityInventoryAgentInterface* Buyer = GetOwnerObject<IAbilityInventoryAgentInterface>();
				IAbilityInventoryAgentInterface* Seller = TransactionTarget;

				FAbilityItem _TransactionItem = _SelectedTransactionItem;
				TArray<FAbilityItem> _PreviewItems = SelectedPreviewItems;

				Buyer->GetInventory()->RemoveItemByRange(_TransactionItem);

				for(auto Iter : _PreviewItems)
				{
					Buyer->GetInventory()->RemoveItemByRange(Iter);
				}

				for(auto Iter : _PreviewItems)
				{
					Seller->GetInventory()->AddItemByRange(Iter);
				}

				GetWorld()->GetTimerManager().ClearTimer(GenerateRawDataRefreshTH);
	
				FSimpleDelegate OnCompleted = FSimpleDelegate::CreateLambda([this, Buyer](){
					bool bCanTransaction;
					FAbilityItem _SelectedTransactionItem;
					if(GetSelectedTransactionItem(_SelectedTransactionItem, bCanTransaction))
					{
						FAbilityItem _GenerateItem = _SelectedTransactionItem.GetData<UDWPropBlueprintData>().GetGenerateItem(_SelectedTransactionItem.Level, UVoxelModule::Get().GetWorldData().RandomStream);
						Buyer->GetInventory()->AddItemByRange(_GenerateItem, -1);
					}
					OnTransactionContentRefresh();
					GetWorld()->GetTimerManager().SetTimer(GenerateRawDataRefreshTH, FTimerDelegate::CreateUObject(this, &UWidgetTransactionPanel::OnGenerateRawDataRefresh), 1.5f, true);
				});
				UWidgetModuleStatics::OpenUserWidget<UWidgetProgressBox>({ FString::Printf(TEXT("%s×%d"), TEXT("未知"), GetSelectedTransactionNum()), 2.f, true, &OnCompleted });
				break;
			}
			case EDWTransactionType::Split:
			{
				IAbilityInventoryAgentInterface* Buyer = GetOwnerObject<IAbilityInventoryAgentInterface>();
		
				FAbilityItem _TransactionItem = _SelectedTransactionItem;

				Buyer->GetInventory()->RemoveItemByRange(_TransactionItem);

				FSimpleDelegate OnCompleted = FSimpleDelegate::CreateLambda([this, Buyer](){
					TArray<FAbilityItem> _PreviewItems = SelectedPreviewItems;
					for(auto Iter : _PreviewItems)
					{
						Buyer->GetInventory()->AddItemByRange(Iter);
					}
					OnTransactionContentRefresh();
				});
				UWidgetModuleStatics::OpenUserWidget<UWidgetProgressBox>({ FText::FromString(FString::Printf(TEXT("%s×%d"), *_SelectedTransactionItem.GetData().Name.ToString(), GetSelectedTransactionNum())), FText::FromString(TEXT("拆解物品")), GetSelectedTransactionNum() * 1.f, true, &OnCompleted });
				break;
			}
			default: break;
		}

		UAchievementModuleStatics::UnlockAchievement(FName("FirstTransactionItem"));
	}
}

void UWidgetTransactionPanel::OnTransactionNumOptionValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue)
{
	Refresh();
}

EDWTransactionType UWidgetTransactionPanel::GetSelectedTabType() const
{
	return (EDWTransactionType)TabGroup->GetSelectedButtonIndex();
}

void UWidgetTransactionPanel::SetSelectedTabType(EDWTransactionType InIndex) const
{
	TabGroup->SelectButtonAtIndex((int32)InIndex);
}

int32 UWidgetTransactionPanel::GetSelectedTransactionNum() const
{
	if(TransactionNumOption)
	{
		return FCString::Atoi(*TransactionNumOption->GetValue().GetStringValue());
	}
	return 0;
}

bool UWidgetTransactionPanel::GetSelectedTransactionItem(FAbilityItem& OutItemData, bool& bCanTransaction) const
{
	if(SelectedTransactionItem)
	{
		OutItemData = SelectedTransactionItem->GetItem();
		switch(GetSelectedTabType())
		{
			case EDWTransactionType::Buy:
			case EDWTransactionType::Sell:
			case EDWTransactionType::Split:
			{
				bCanTransaction = OutItemData.Count >= GetSelectedTransactionNum();
				OutItemData.Count = GetSelectedTransactionNum();
				break;
			}
			case EDWTransactionType::Upgrade:
			{
				bCanTransaction = OutItemData.Level + GetSelectedTransactionNum() <= OutItemData.GetData().MaxLevel;
				OutItemData.Level += GetSelectedTransactionNum();
				break;
			}
			case EDWTransactionType::Generate:
			{
				bCanTransaction = true;
				break;
			}
			default: break;
		}
		return true;
	}
	return false;
}
