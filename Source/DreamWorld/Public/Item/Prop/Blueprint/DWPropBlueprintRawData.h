// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWPropBlueprintData.h"
#include "DWPropBlueprintRawData.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWPropBlueprintRawData : public UDWPropBlueprintData
{
	GENERATED_BODY()

public:
	UDWPropBlueprintRawData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWRawType GenerateRawType;

protected:
	virtual bool CanGenerateItem(UAbilityItemDataBase* InItemData) const override;
};
