#pragma once

#include "DWCharacterAbility.h"
#include "DreamWorld/DreamWorld.h"
#include "DWCharacterAttackAbility.generated.h"

/**
 * 角色工具Ability基类
 */
UCLASS()
class DREAMWORLD_API UDWCharacterAttackAbility : public UDWCharacterAbility
{
	GENERATED_BODY()

public:
	UDWCharacterAttackAbility();
};