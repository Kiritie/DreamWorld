#include "Ability/Character/DWCharacterActionAbility.h"

#include "Character/DWCharacter.h"

UDWCharacterActionAbility::UDWCharacterActionAbility()
{
	ActionType = EDWCharacterActionType::None;
}

void UDWCharacterActionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if(ADWCharacter* OwnerCharacter = GetOwnerCharacter<ADWCharacter>())
	{
		//OwnerCharacter->EndAction(ActionType);
	}
}
