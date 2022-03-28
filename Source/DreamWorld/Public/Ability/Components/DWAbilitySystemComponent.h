#pragma once

#include "Ability/Components/AbilitySystemComponentBase.h"
#include "DWAbilitySystemComponent.generated.h"

class UAbilityBase;

/**
 * AbilitySystem组件
 */
UCLASS()
class DREAMWORLD_API UDWAbilitySystemComponent : public UAbilitySystemComponentBase
{
	GENERATED_BODY()

public:
	UDWAbilitySystemComponent();
};