// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Vitality/AbilityVitalityInventoryBase.h"
#include "DWVitalityInventory.generated.h"

/**
 * �����Ʒ��
 */
UCLASS()
class DREAMWORLD_API UDWVitalityInventory : public UAbilityVitalityInventoryBase
{
	GENERATED_BODY()

public:
	UDWVitalityInventory();

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void UnloadData(EPhase InPhase) override;
};
