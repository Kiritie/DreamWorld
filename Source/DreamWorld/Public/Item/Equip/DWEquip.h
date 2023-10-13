// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "DWEquip.generated.h"

/**
 * װ��
 */
UCLASS()
class DREAMWORLD_API ADWEquip : public AAbilityEquipBase
{
	GENERATED_BODY()
	
public:	
	ADWEquip();

public:
	virtual void Initialize_Implementation(AActor* InOwnerActor, const FAbilityItem& InItem = FAbilityItem::Empty) override;

public:
	virtual void OnAssemble_Implementation() override;
	
	virtual void OnDischarge_Implementation() override;
};
