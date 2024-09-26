#include "Ability/Character/DWCharacterActionAbility.h"

#include "Character/DWCharacter.h"

UDWCharacterActionAbility::UDWCharacterActionAbility()
{
	bWasStopped = false;
}

void UDWCharacterActionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDWCharacterActionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ADWCharacter* OwnerCharacter = GetOwnerActor<ADWCharacter>();

	if(!OwnerCharacter) return;
	
	if(!bWasStopped)
	{
		OwnerCharacter->EndAction(AbilityTags.GetByIndex(0), bWasCancelled);
	}
}
