// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWPropBlueprintData.h"
#include "DWPropBlueprintWeaponData.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWPropBlueprintWeaponData : public UDWPropBlueprintData
{
	GENERATED_BODY()

public:
	UDWPropBlueprintWeaponData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWWeaponType GenerateWeaponType;

protected:
	virtual bool CanGenerateItem(UAbilityItemDataBase* InItemData) const override;
};
