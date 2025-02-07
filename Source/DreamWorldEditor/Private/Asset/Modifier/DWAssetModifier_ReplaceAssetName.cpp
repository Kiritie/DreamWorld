// Fill out your copyright notice in the Description page of Project Settings.

#include "Asset/Modifier/DWAssetModifier_ReplaceAssetName.h"

#include "AssetToolsModule.h"

UDWAssetModifier_ReplaceAssetName::UDWAssetModifier_ReplaceAssetName()
{
	OldName = TEXT("");
	NewName = TEXT("");
}

bool UDWAssetModifier_ReplaceAssetName::CanModify_Implementation(const FAssetData& InAssetData) const
{
	if(UObject* Asset = InAssetData.GetAsset())
	{
		if(!OldName.IsEmpty() && Asset->GetName().Contains(OldName) && !NewName.IsEmpty() && OldName != NewName)
		{
			return true;
		}
	}
	return false;
}

void UDWAssetModifier_ReplaceAssetName::DoModify_Implementation(const FAssetData& InAssetData)
{
	if(UObject* Asset = InAssetData.GetAsset())
	{
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		const FString PackagePath = FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName());
		FAssetRenameData AssetRenameData = FAssetRenameData(Asset, PackagePath, Asset->GetName().Replace(*OldName, *NewName));
		AssetToolsModule.Get().RenameAssets({ AssetRenameData });

		Super::DoModify_Implementation(InAssetData);
	}
}
