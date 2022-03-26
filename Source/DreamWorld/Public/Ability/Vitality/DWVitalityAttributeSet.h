#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Ability/Vitality/VitalityAttributeSetBase.h"
#include "DWVitalityAttributeSet.generated.h"

/**
 * 角色属性集
 */
UCLASS()
class DREAMWORLD_API UDWVitalityAttributeSet : public UVitalityAttributeSetBase
{
	GENERATED_BODY()

public:
	UDWVitalityAttributeSet();

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
};