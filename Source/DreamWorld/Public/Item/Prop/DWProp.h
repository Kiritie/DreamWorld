// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Prop/AbilityPropBase.h"
#include "DWProp.generated.h"

/**
 * װ��
 */
UCLASS()
class DREAMWORLD_API ADWProp : public AAbilityPropBase
{
	GENERATED_BODY()
	
public:	
	ADWProp();

public:
	virtual void Initialize_Implementation(AAbilityCharacterBase* InOwnerCharacter, const FAbilityItem& InItem = FAbilityItem::Empty) override;
};
