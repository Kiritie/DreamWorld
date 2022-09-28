#pragma once

#include "DWCharacterAbility.h"
#include "DreamWorld/DreamWorld.h"
#include "DWCharacterActionAbility.generated.h"

/**
 * ��ɫ��ΪAbility����
 */
UCLASS()
class DREAMWORLD_API UDWCharacterActionAbility : public UDWCharacterAbility
{
	GENERATED_BODY()

public:
	UDWCharacterActionAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	//UPROPERTY(BlueprintReadWrite)
	EDWCharacterActionType ActionType;

	//UPROPERTY(BlueprintReadWrite)
	bool bWasStopped;

public:
	EDWCharacterActionType GetActionType() const { return ActionType; }

	void SetActionType(EDWCharacterActionType InActionType) { ActionType = InActionType; }

	bool WasStopped() const { return bWasStopped; }

	void SetStopped(bool val) { bWasStopped = val; }
};
