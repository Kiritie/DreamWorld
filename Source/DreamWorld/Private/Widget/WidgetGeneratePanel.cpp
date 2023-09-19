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
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"

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

	FPrimaryAssetId GenerateToolID = FPrimaryAssetId();

	if(InParams.IsValidIndex(0))
	{
		if(AVoxelInteractAuxiliary* InteractionAgent = InParams[0].GetObjectValue<AVoxelInteractAuxiliary>())
		{
			GenerateToolID = InteractionAgent->GetVoxelItem().ID;
		}
	}

	TArray<FDWGenerateItemData> GenerateItemDatas;
	if(GenerateContent && UAssetModuleBPLibrary::ReadDataTable(GenerateItemDatas))
	{
		for(auto Iter : GenerateItems)
		{
			DestroySubWidget(Iter, true);
		}
		GenerateItems.Empty();
		for(auto Iter : GenerateItemDatas)
		{
			if(Iter.ToolID.IsValid() && Iter.ToolID != GenerateToolID) continue;
			
			if(UWidgetInventoryGenerateItem* GenerateItem = CreateSubWidget<UWidgetInventoryGenerateItem>({ &Iter.Item, &Iter}, GenerateSlotClass))
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

	if(WidgetParams.IsValidIndex(0))
	{
		if(AVoxelInteractAuxiliary* InteractionAgent = WidgetParams[0].GetObjectValue<AVoxelInteractAuxiliary>())
		{
			GetOwnerObject<IInteractionAgentInterface>()->DoInteract((EInteractAction)EVoxelInteractAction::Close, InteractionAgent);
		}
	}
	PreviewGenerateRawDataIndex = 0;
	SelectedGenerateRawData = FDWGenerateRawData();
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
			for(auto& Iter1 : GenerateItemData.RawDatas)
			{
				for(auto& Iter2 : Iter1.Raws)
				{
					const FQueryItemInfo QueryItemInfo = Inventory->QueryItemByRange(EQueryItemType::Get, Iter2);
					if(QueryItemInfo.Item.Count < Iter2.Count)
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

void UWidgetGeneratePanel::OnGenerateSlotSelected_Implementation(UWidgetInventoryGenerateItem* InSlot)
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
				if(UWidgetInventoryPreviewItem* PreviewItem = CreateSubWidget<UWidgetInventoryPreviewItem>({ &Iter }, PreviewSlotClass))
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
		UInventory* Inventory = Cast<ADWCharacter>(OwnerObject)->GetInventory();
		for(auto& Iter : SelectedGenerateRawData.Raws)
		{
			Inventory->RemoveItemByRange(Iter);
		}
		Inventory->AddItemByRange(GenerateItemData.Item, -1);

		UGlobalBPLibrary::GetGameInstance()->GetSubsystem<UAchievementSubSystem>()->Unlock(FName("FirstGenerateItem"));
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
