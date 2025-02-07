// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Asset/Modifier/AssetModifierBase.h"
#include "DWAssetModifier_ReplaceAssetName.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DREAMWORLDEDITOR_API UDWAssetModifier_ReplaceAssetName : public UAssetModifierBase
{
	GENERATED_BODY()

public:
	UDWAssetModifier_ReplaceAssetName();

public:
	virtual bool CanModify_Implementation(const FAssetData& InAssetData) const override;

	virtual void DoModify_Implementation(const FAssetData& InAssetData) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OldName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NewName;
};
