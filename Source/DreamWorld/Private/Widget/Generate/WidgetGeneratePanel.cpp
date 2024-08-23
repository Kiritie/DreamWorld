// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Generate/WidgetGeneratePanel.h"

#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Ability/Inventory/Slot/AbilityInventorySlot.h"
#include "Asset/AssetModuleStatics.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/ScrollBox.h"
#include "CommonButtonBase.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Achievement/AchievementModuleStatics.h"
#include "Character/DWCharacter.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Generate/WidgetGenerateItem.h"
#include "Widget/Item/WidgetAbilityPreviewItem.h"

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
}

void UWidgetGeneratePanel::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
	
	if(BtnGenerate)
	{
		BtnGenerate->OnClicked().AddUObject(this, &UWidgetGeneratePanel::OnGenerateButtonClicked);
	}
}

void UWidgetGeneratePanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	if(OwnerObject)
	{
		UAbilityInventoryBase* Inventory = Cast<ADWCharacter>(OwnerObject)->GetInventory();
		Inventory->OnRefresh.RemoveDynamic(this, &UWidgetGeneratePanel::Refresh);
	}

	Super::OnInitialize(InOwner, InParams);

	if(InOwner)
	{
		UAbilityInventoryBase* Inventory = Cast<ADWCharacter>(InOwner)->GetInventory();
		Inventory->OnRefresh.AddDynamic(this, &UWidgetGeneratePanel::Refresh);
	}
}

void UWidgetGeneratePanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	const FPrimaryAssetId GenerateToolID = InParams.IsValidIndex(0) ? InParams[0].GetObjectValue<IPrimaryEntityInterface>()->Execute_GetAssetID(InParams[0]) : FPrimaryAssetId();

	TArray<FDWGenerateItemData> GenerateItemDatas;
	if(GenerateContent && UAssetModuleStatics::ReadDataTable(GenerateItemDatas))
	{
		for(auto Iter : GenerateItems)
		{
			DestroySubWidget(Iter, true);
		}
		GenerateItems.Empty();
		for(auto& Iter : GenerateItemDatas)
		{
			if(Iter.ToolID.IsValid() && Iter.ToolID != GenerateToolID) continue;
			
			if(UWidgetGenerateItem* GenerateItem = CreateSubWidget<UWidgetGenerateItem>({ &Iter.Item, &Iter}, UAssetModuleStatics::GetStaticClass(FName("GenerateItem"))))
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

void UWidgetGeneratePanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	PreviewGenerateRawDataIndex = 0;
	SelectedGenerateRawData = FDWGenerateRawData();
}

void UWidgetGeneratePanel::OnRefresh()
{
	Super::OnRefresh();

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
			UAbilityInventoryBase* Inventory = Cast<ADWCharacter>(OwnerObject)->GetInventory();
			for(auto& Iter1 : GenerateItemData.RawDatas)
			{
				for(auto& Iter2 : Iter1.Raws)
				{
					const FItemQueryInfo ItemQueryInfo = Inventory->QueryItemByRange(EItemQueryType::Get, Iter2);
					if(ItemQueryInfo.Item.Count < Iter2.Count)
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
		BtnGenerate->SetIsEnabled(bCanGenerate);
	}
}

void UWidgetGeneratePanel::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);
	
	for(auto Iter : GenerateItems)
	{
		DestroySubWidget(Iter, true);
	}

	for(auto Iter : PreviewItems)
	{
		DestroySubWidget(Iter, true);
	}
}

void UWidgetGeneratePanel::OnGenerateSlotSelected_Implementation(UWidgetGenerateItem* InSlot)
{
	if(InSlot == SelectedGenerateSlot) return;

	if(SelectedGenerateSlot)
	{
		SelectedGenerateSlot->OnUnSelected();
	}

	SelectedGenerateSlot = InSlot;

	if(SelectedGenerateSlot)
	{
		PreviewGenerateRawDataIndex = 0;
		GetWorld()->GetTimerManager().SetTimer(PreviewContentRefreshTH, FTimerDelegate::CreateUObject(this, &UWidgetGeneratePanel::OnPreviewContentRefresh), 1.5f, true, 0.f);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(PreviewContentRefreshTH);
		for(auto Iter : PreviewItems)
		{
			DestroySubWidget(Iter, true);
		}
		PreviewItems.Empty();
	}

	Refresh();
}

void UWidgetGeneratePanel::OnPreviewContentRefresh()
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
			for(auto& Iter : GenerateItemData.RawDatas[PreviewGenerateRawDataIndex].Raws)
			{
				if(UWidgetAbilityPreviewItem* PreviewItem = CreateSubWidget<UWidgetAbilityPreviewItem>({ &Iter }, UAssetModuleStatics::GetStaticClass(FName("PreviewItem"))))
				{
					if(UWrapBoxSlot* WrapBoxSlot = PreviewContent->AddChildToWrapBox(PreviewItem))
					{
						WrapBoxSlot->SetPadding(FMargin(5.f, 5.f, 5.f, 5.f));
					}
					PreviewItems.Add(PreviewItem);
				}
			}
			if(GenerateItemData.RawDatas.Num() == 1)
			{
				GetWorld()->GetTimerManager().ClearTimer(PreviewContentRefreshTH);
			}
			else if(++PreviewGenerateRawDataIndex >= GenerateItemData.RawDatas.Num())
			{
				PreviewGenerateRawDataIndex = 0;
			}
		}
	}
}

void UWidgetGeneratePanel::OnGenerateButtonClicked()
{
	FDWGenerateItemData GenerateItemData;
	if(GetSelectedGenerateItemData(GenerateItemData))
	{
		UAbilityInventoryBase* Inventory = Cast<ADWCharacter>(OwnerObject)->GetInventory();
		for(auto& Iter : SelectedGenerateRawData.Raws)
		{
			Inventory->RemoveItemByRange(Iter);
		}
		Inventory->AddItemByRange(GenerateItemData.Item, -1);
		if(GenerateItemData.Item.Count > 0)
		{
			Cast<ADWCharacter>(OwnerObject)->OnDiscardItem(GenerateItemData.Item, false);
		}
		UAchievementModuleStatics::UnlockAchievement(FName("FirstGenerateItem"));
	}
}

bool UWidgetGeneratePanel::GetSelectedGenerateItemData(FDWGenerateItemData& OutItemData) const
{
	if(SelectedGenerateSlot)
	{
		OutItemData = SelectedGenerateSlot->GenerateItemData;
		return true;
	}
	return false;
}
