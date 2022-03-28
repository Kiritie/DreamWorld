#pragma once

#include "Ability/Character/CharacterAbilityBase.h"
#include "DreamWorld/DreamWorld.h"
#include "DWCharacterAbility.generated.h"

/**
 * 角色Ability基类
 */
UCLASS()
class DREAMWORLD_API UDWCharacterAbility : public UCharacterAbilityBase
{
	GENERATED_BODY()

public:
	UDWCharacterAbility();
};