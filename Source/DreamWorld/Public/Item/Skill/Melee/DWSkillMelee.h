// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Skill/Melee/AbilitySkillMeleeBase.h"
#include "DWSkillMelee.generated.h"

/**
 * װ��
 */
UCLASS()
class DREAMWORLD_API ADWSkillMelee : public AAbilitySkillMeleeBase
{
	GENERATED_BODY()
	
public:	
	ADWSkillMelee();

public:
	virtual void Initialize(AAbilityCharacterBase* InOwnerCharacter) override;
};
