// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DWCommonTypes.h"
#include "Common/CommonStatics.h"
#include "DWCommonStatics.generated.h"

UCLASS()
class DREAMWORLD_API UDWCommonStatics : public UCommonStatics
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "DWCommonStatics")
	static EDWEquipPart GetClampedEquipPart(EDWEquipPart InEquipPart);

	UFUNCTION(BlueprintPure, Category = "DWCommonStatics")
	static EDWEquipPart GetMatchedWeaponPart(EDWEquipPart InEquipPart);

	UFUNCTION(BlueprintPure, Category = "DWCommonStatics")
	static TArray<EDWEquipPart> GetWeaponPartsByGroup(EDWWeaponGroup InWeaponGroup);
};
