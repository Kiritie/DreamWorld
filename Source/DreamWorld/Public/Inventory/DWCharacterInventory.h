// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Character/AbilityCharacterInventoryBase.h"
#include "Common/DWCommonTypes.h"
#include "DWCharacterInventory.generated.h"

class UAbilityInventoryEquipSlotBase;

/**
 * ��ɫ��Ʒ��
 */
UCLASS()
class DREAMWORLD_API UDWCharacterInventory : public UAbilityCharacterInventoryBase
{
	GENERATED_BODY()

public:
	UDWCharacterInventory();

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void UnloadData(EPhase InPhase) override;

public:
	virtual UAbilityInventoryEquipSlotBase* GetEquipSlotByPart(EDWEquipPart InEquipPart);
};
