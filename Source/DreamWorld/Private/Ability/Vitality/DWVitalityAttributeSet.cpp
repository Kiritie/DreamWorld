#include "Ability/Vitality/DWVitalityAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Character/DWCharacter.h"
#include "Common/DWCommonTypes.h"

UDWVitalityAttributeSet::UDWVitalityAttributeSet()
{
	DamageHandle = UDWDamageHandle::StaticClass();
}

void UDWVitalityAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UDWVitalityAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
