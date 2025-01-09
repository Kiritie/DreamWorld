// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/DWCommonTypes.h"
#include "Item/Prop/DWPropData.h"
#include "DWPropBlueprintData.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWPropBlueprintData : public UDWPropData
{
	GENERATED_BODY()

public:
	UDWPropBlueprintData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityItemType GenerateItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDWGenerateRawData> GenerateRawDatas;

public:
	UFUNCTION(BlueprintPure)
	virtual FAbilityItem GetGenerateItem(int32 InLevel, FRandomStream InRandomStream = FRandomStream()) const;

protected:
	virtual bool CanGenerateItem(UAbilityItemDataBase* InItemData) const;
};
