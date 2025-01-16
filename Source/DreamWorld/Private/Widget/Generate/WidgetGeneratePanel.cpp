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
#include "Ability/Item/AbilityItemDataBase.h"
#include "Achievement/AchievementModuleStatics.h"
#include "Setting/Widget/Item/WidgetOptionSettingItemBase.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Common/WidgetProgressBox.h"
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
	if(GenerateNumOption)
	{
		GenerateNumOption->OnValueChanged.AddDynamic(this, &UWidgetGeneratePanel::OnGenerateNumOptionValueChange);
	}
}

void UWidgetGeneratePanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetGeneratePanel::OnReset(bool bForce)
{
	Super::OnReset(bForce);

	OnGenerateItemDeselected(SelectedGenerateItem);

	for(auto Iter : GenerateItems)
	{
		DestroySubWidget(Iter, true);
	}
	GenerateItems.Empty();
}

void UWidgetGeneratePanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	if(InParams.IsValidIndex(0))
	{
		GenerateToolID = InParams[0].GetObjectValue<IPrimaryEntityInterface>()->Execute_GetAssetID(InParams[0]);
	}
	
	if(CategoryBar)
	{
		CategoryBar->SetSelectedItemType(EAbilityItemType::None);
	}

	OnGenerateContentRefresh(true);

	Super::OnOpen(InParams, bInstant);

	if(GetOwnerObject<IAbilityInventoryAgentInterface>())
	{
		GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory()->OnRefresh.AddDynamic(this, &UWidgetGeneratePanel::Refresh);
	}

	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();
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
	FDWGenerateItemData _SelectedGenerateItemData;
	if(GetSelectedGenerateItemData(_SelectedGenerateItemData))
	{
		UAbilityInventoryBase* Inventory = GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory();
		SelectedPreviewItems = _SelectedGenerateItemData.RawDatas[GenerateRawDataIndex].Raws;
		for(auto& Iter : SelectedPreviewItems)
		{
			Iter.Count *= GetSelectedGenerateNum();
			const FItemQueryData ItemQueryData = Inventory->QueryItemByRange(EItemQueryType::Get, Iter);
			if(ItemQueryData.Item.Count < Iter.Count)
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
	
	if(BtnGenerate)
	{
		// BtnGenerate->SetIsEnabled(bCanGenerate);
		BtnGenerate->SetRenderOpacity(bCanGenerate ? 1.f : 0.5f);
		BtnGenerate->SetVisibility(bCanGenerate ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::HitTestInvisible);
	}

	OnPreviewContentRefresh();
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

	GenerateRawDataIndex = 0;

	if(GenerateNumOption)
	{
		TArray<FString> Options;
		DON_WITHINDEX(20, i, Options.Add(FString::FromInt(SelectedGenerateItem->GetGenerateItemData().Item.Count * (i + 1))); )
		GenerateNumOption->SetOptionNames(Options);
	}

	Refresh();

	GetWorld()->GetTimerManager().SetTimer(GenerateRawDataRefreshTH, FTimerDelegate::CreateUObject(this, &UWidgetGeneratePanel::OnGenerateRawDataRefresh), 1.5f, true);
}

void UWidgetGeneratePanel::OnGenerateItemDeselected_Implementation(UWidgetGenerateItem* InItem)
{
	if(InItem != SelectedGenerateItem) return;

	SelectedGenerateItem = nullptr;

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

void UWidgetGeneratePanel::OnGenerateContentRefresh(bool bScrollToStart)
{
	Reset();

	TArray<FDWGenerateItemData> _GenerateItemDatas;
	if(GenerateContent && UAssetModuleStatics::ReadDataTable(_GenerateItemDatas))
	{
		for(auto& Iter : _GenerateItemDatas)
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

void UWidgetGeneratePanel::OnGenerateRawDataRefresh()
{
	FDWGenerateItemData _SelectedGenerateItemData;
	if(GetSelectedGenerateItemData(_SelectedGenerateItemData))
	{
		if(++GenerateRawDataIndex >= _SelectedGenerateItemData.RawDatas.Num())
		{
			GenerateRawDataIndex = 0;
		}
		Refresh();
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
			for(auto& Iter : SelectedPreviewItems)
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
		}
	}
}

void UWidgetGeneratePanel::OnGenerateButtonClicked()
{
	FDWGenerateItemData _SelectedGenerateItemData;
	if(GetSelectedGenerateItemData(_SelectedGenerateItemData))
	{
		UAbilityInventoryBase* Inventory = GetOwnerObject<IAbilityInventoryAgentInterface>()->GetInventory();
		for(auto Iter : SelectedPreviewItems)
		{
			Inventory->RemoveItemByRange(Iter);
		}
		GetWorld()->GetTimerManager().ClearTimer(GenerateRawDataRefreshTH);
	
		FSimpleDelegate OnCompleted = FSimpleDelegate::CreateLambda([this, Inventory](){
			FDWGenerateItemData _SelectedGenerateItemData;
			if(GetSelectedGenerateItemData(_SelectedGenerateItemData))
			{
				Inventory->AddItemByRange(_SelectedGenerateItemData.Item, -1);
				UAchievementModuleStatics::UnlockAchievement(FName("FirstGenerateItem"));
			}
			GetWorld()->GetTimerManager().SetTimer(GenerateRawDataRefreshTH, FTimerDelegate::CreateUObject(this, &UWidgetGeneratePanel::OnGenerateRawDataRefresh), 1.5f, true);
		});
		UWidgetModuleStatics::OpenUserWidget<UWidgetProgressBox>({ FText::FromString(FString::Printf(TEXT("%s×%d"), *_SelectedGenerateItemData.Item.GetData().Name.ToString(), GetSelectedGenerateNum(true))), FText::FromString(TEXT("制作物品")), GetSelectedGenerateNum() * 1.f, true, &OnCompleted });
	}
}

void UWidgetGeneratePanel::OnGenerateNumOptionValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue)
{
	Refresh();
}

int32 UWidgetGeneratePanel::GetSelectedGenerateNum(bool bReal) const
{
	if(GenerateNumOption && SelectedGenerateItem)
	{
		return FCString::Atoi(*GenerateNumOption->GetValue().GetStringValue()) / (bReal ? 1 : SelectedGenerateItem->GetItem().Count);
	}
	return 0;
}

bool UWidgetGeneratePanel::GetSelectedGenerateItemData(FDWGenerateItemData& OutItemData) const
{
	if(SelectedGenerateItem)
	{
		OutItemData = SelectedGenerateItem->GetGenerateItemData();
		OutItemData.Item.Count *= GetSelectedGenerateNum();
		return true;
	}
	return false;
}
