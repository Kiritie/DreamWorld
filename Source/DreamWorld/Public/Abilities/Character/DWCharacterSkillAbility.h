#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Abilities/Character/DWCharacterAbility.h"
#include "DWCharacterSkillAbility.generated.h"

/**
 * 角色技能Ability基类
 */
UCLASS()
class DREAMWORLD_API UDWCharacterSkillAbility : public UDWCharacterAbility
{
	GENERATED_BODY()

public:
	UDWCharacterSkillAbility();
};