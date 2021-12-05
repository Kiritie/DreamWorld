#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DWDamageExecution.generated.h"

/**
 * �˺����㼰ִ����
 */
UCLASS()
class DREAMWORLD_API UDWDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDWDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};