// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Skill/Remote/AbilitySkillRemoteBase.h"
#include "DWSkillRemote.generated.h"

/**
 * װ��
 */
UCLASS()
class DREAMWORLD_API ADWSkillRemote : public AAbilitySkillRemoteBase
{
	GENERATED_BODY()
	
public:	
	ADWSkillRemote();

public:
	virtual void Initialize(AAbilityCharacterBase* InOwnerCharacter) override;
};
