// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWPropBlueprintData.h"
#include "DWPropBlueprintArmorData.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWPropBlueprintArmorData : public UDWPropBlueprintData
{
	GENERATED_BODY()

public:
	UDWPropBlueprintArmorData();

protected:
	virtual bool CanGenerateItem(UAbilityItemDataBase* InItemData) const override;
};
