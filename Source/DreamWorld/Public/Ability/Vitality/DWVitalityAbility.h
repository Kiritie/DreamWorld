#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Ability/Vitality/VitalityAbilityBase.h"
#include "DWVitalityAbility.generated.h"

/**
 * Vitality Ability基类
 */
UCLASS()
class DREAMWORLD_API UDWVitalityAbility : public UVitalityAbilityBase
{
	GENERATED_BODY()

public:
	UDWVitalityAbility();
};