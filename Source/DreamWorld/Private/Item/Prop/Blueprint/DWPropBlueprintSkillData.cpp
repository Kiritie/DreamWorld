// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Prop/Blueprint/DWPropBlueprintSkillData.h"

#include "Item/Skill/DWSkillData.h"

UDWPropBlueprintSkillData::UDWPropBlueprintSkillData()
{
	GenerateSkillType = EDWSkillType::None;
}

bool UDWPropBlueprintSkillData::CanGenerateItem(UAbilityItemDataBase* InItemData) const
{
	if(!Super::CanGenerateItem(InItemData)) return false;

	if(UDWSkillData* SkillData = Cast<UDWSkillData>(InItemData))
	{
		return SkillData->SkillType == GenerateSkillType;
	}
	return false;
}
