// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWPropBlueprintData.h"
#include "DWPropBlueprintPropData.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWPropBlueprintPropData : public UDWPropBlueprintData
{
	GENERATED_BODY()

public:
	UDWPropBlueprintPropData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWPropType GeneratePropType;

protected:
	virtual bool CanGenerateItem(UAbilityItemDataBase* InItemData) const override;
};
