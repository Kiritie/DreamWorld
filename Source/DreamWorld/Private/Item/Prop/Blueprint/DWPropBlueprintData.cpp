// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Prop/Blueprint/DWPropBlueprintData.h"

#include "Asset/AssetModuleStatics.h"
#include "Common/CommonStatics.h"

UDWPropBlueprintData::UDWPropBlueprintData()
{
	PropType = EDWPropType::Blueprint;
	
	GenerateItemType = EAbilityItemType::None;
	GenerateRawDatas = TArray<FDWGenerateRawData>();
}

FAbilityItem UDWPropBlueprintData::GetGenerateItem(int32 InLevel, FRandomStream InRandomStream) const
{
	const FName _ItemType = *UCommonStatics::GetEnumAuthoredNameByValue(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)GenerateItemType);

	TArray<UAbilityItemDataBase*> ItemDatas;
	for(auto Iter : UAssetModuleStatics::LoadPrimaryAssets<UAbilityItemDataBase>(_ItemType))
	{
		if(CanGenerateItem(Iter))
		{
			ItemDatas.Add(Iter);
		}
	}
	if(ItemDatas.Num() > 0)
	{
		FAbilityItem GenerateItem = FAbilityItem(ItemDatas[InRandomStream.RandRange(0, ItemDatas.Num() - 1)]->GetPrimaryAssetId(), 1, InLevel);
		GenerateItem.Level = InLevel;
		return GenerateItem;
	}
	return FAbilityItem::Empty;
}

bool UDWPropBlueprintData::CanGenerateItem(UAbilityItemDataBase* InItemData) const
{
	return InItemData->Rarity == Rarity;
}
