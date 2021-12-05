#pragma once

#include "DreamWorld/DreamWorld.h"
#include "DWItemAbility.h"
#include "DWSkillAbility.generated.h"

/**
 * 道具Ability基类
 */
UCLASS()
class DREAMWORLD_API UDWSkillAbility : public UDWItemAbility
{
	GENERATED_BODY()

public:
	UDWSkillAbility();
};