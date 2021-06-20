#pragma once

#include "DreamWorld.h"
#include "AbilitySystemComponent.h"
#include "DWAbilitySystemComponent.generated.h"

class UDWGameplayAbility;

/**
 * AbilitySystem���
 */
UCLASS()
class DREAMWORLD_API UDWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UDWAbilitySystemComponent();

	/** ���ݱ�ǩ��ȡ��ǰ�����Ability�б� */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer & AbilityTags, TArray<UDWGameplayAbility*>& ActiveAbilities);

	/** ��ȡAbility��Ĭ�ϵȼ����ɽ�ɫ���� */
	int32 GetDefaultAbilityLevel() const;

public:
	/** ͨ��Actor��ȡAbilitySystemComponent */
	static UDWAbilitySystemComponent* GetAbilitySystemComponentFormActor(const AActor* Actor, bool LookForComponent = false);
};