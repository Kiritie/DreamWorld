#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Ability/Item/Skill/SkillAbilityBase.h"
#include "DWSkillAbility.generated.h"

/**
 * 道具Ability基类
 */
UCLASS()
class DREAMWORLD_API UDWSkillAbility : public USkillAbilityBase
{
	GENERATED_BODY()

public:
	UDWSkillAbility();
};