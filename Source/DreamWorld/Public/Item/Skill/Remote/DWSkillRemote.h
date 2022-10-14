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
	virtual void Initialize_Implementation(AAbilityCharacterBase* InOwnerCharacter, const FAbilityItem& InItem = FAbilityItem::Empty) override;

public:
	virtual void OnHitTarget_Implementation(AActor* InTarget, const FHitResult& InHitResult);
};
