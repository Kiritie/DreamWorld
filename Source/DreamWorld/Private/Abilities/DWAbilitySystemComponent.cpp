#include "Abilities/DWAbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/DWGameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "Character/DWCharacter.h"
#include "GameplayAbilitySpec.h"

UDWAbilitySystemComponent::UDWAbilitySystemComponent() { }

void UDWAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& AbilityTags, TArray<UDWGameplayAbility*>& ActiveAbilities)
{
	// ��ȡ��ǩ��Ӧ���Ѽ����Ability�б�
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, AbilitiesToActivate, false);

	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// ��ȡAbilityʵ���б�
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			// ��Abilityʵ�����뷵���б���
			ActiveAbilities.Add(Cast<UDWGameplayAbility>(ActiveAbility));
		}
	}
}

int32 UDWAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwnerActor());
	if (OwnerCharacter)
	{
		return OwnerCharacter->GetLevelC();
	}
	return 1;
}

UDWAbilitySystemComponent* UDWAbilitySystemComponent::GetAbilitySystemComponentFormActor(const AActor* Actor, bool LookForComponent /*= false*/)
{
	return Cast<UDWAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
