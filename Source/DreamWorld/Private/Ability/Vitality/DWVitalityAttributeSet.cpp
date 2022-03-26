#include "Ability/Vitality/DWVitalityAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Character/DWCharacter.h"

UDWVitalityAttributeSet::UDWVitalityAttributeSet()
{
	
}

void UDWVitalityAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UDWVitalityAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
