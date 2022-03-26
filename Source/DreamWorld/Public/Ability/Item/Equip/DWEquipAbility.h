#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Ability/Item/Equip/EquipAbilityBase.h"
#include "DWEquipAbility.generated.h"

/**
 * 装备Ability基类
 */
UCLASS()
class DREAMWORLD_API UDWEquipAbility : public UEquipAbilityBase
{
	GENERATED_BODY()

public:
	UDWEquipAbility();
};