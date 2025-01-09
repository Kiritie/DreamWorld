// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWPropBlueprintData.h"
#include "DWPropBlueprintSkillData.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWPropBlueprintSkillData : public UDWPropBlueprintData
{
	GENERATED_BODY()

public:
	UDWPropBlueprintSkillData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWSkillType GenerateSkillType;

protected:
	virtual bool CanGenerateItem(UAbilityItemDataBase* InItemData) const override;
};
