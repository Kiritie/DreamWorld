#include "Ability/Character/DWCharacterActionAbility.h"

#include "Character/DWCharacter.h"

UDWCharacterActionAbility::UDWCharacterActionAbility()
{
	ActionType = ECharacterActionType::None;
}

void UDWCharacterActionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if(!bWasCancelled)
	{
		if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwnerCharacter()))
		{
			OwnerCharacter->StopAction(ActionType, false, true);
		}
	}
}
